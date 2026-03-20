//
// Created by morphred on 9/11/25.
//

#include "Parser.h"

#include <charconv>
#include <format>
#include <fstream>
#include <vector>
#include <filesystem>

#include "nlohmann/json.hpp"
#include "whereami/whereami.h"

using json = nlohmann::json;

std::string debug_parser(const std::vector<std::string>& arguments)
{
    std::string result;
    for (const auto& argument : arguments)
    {
        result += " " + argument;
    }
    return result;
};

std::string debug_parser(const std::vector<std::vector<std::string>>& arguments)
{
    std::string result;
    for (const auto& argument : arguments)
        result += "\n" + debug_parser(argument);
    return result;
};

std::vector<ArgFormat> Parser::fmt_parse(const std::string& fmt_txt)
{
    std::vector<ArgFormat> format;
    if (fmt_txt.empty())
    {
        const FormatDef fmt{FormatDef::empty};
        format.emplace_back(0, fmt);
        return format;
    }
    int fmt_length;
    int start = 0;
    for (int i = 0; i < fmt_txt.size(); i++)
        if (std::isalpha(fmt_txt[i]))
        {
            if (start == i)
                fmt_length = 1;
            else
                std::from_chars(&fmt_txt[start], &fmt_txt[i], fmt_length);
            start = i + 1;
            if (fmt_txt[i] == 's')
            {
                const FormatDef fmt{FormatDef::string};
                format.push_back({fmt_length, fmt});
            }
            else
            {
                FormatDef::Type fmt_type;
                switch (fmt_txt[i])
                {
                case 'i':
                    fmt_type = FormatDef::integer;
                    break;
                case 'I':
                    fmt_type = FormatDef::u_integer;
                    break;
                case 'b':
                    fmt_type = FormatDef::byte;
                    break;
                case 'B':
                    fmt_type = FormatDef::u_byte;
                    break;
                default:
                    throw std::runtime_error("Invalid format");
                }
                const FormatDef fmt{fmt_type};
                for (int j = 0; j < fmt_length; j++)
                    format.push_back({fmt.size, fmt});
            }
        }
    return format;
}

void Parser::loadAliases()
{
    // TODO alias files
    for (const auto& entry : std::filesystem::directory_iterator(path_db / "named_values"))
    {
        const auto& current_path = entry.path();
        if (entry.is_regular_file())
        {
            auto current_json = json::parse(std::ifstream{current_path});
            auto filename_stem = current_path.filename().stem().string();
            aliasFmt alias{};
            for (auto& [key, value] : current_json.items())
            {
                if (!value.is_string())
                    throw std::runtime_error{"Invalid alias name"};
                alias.emplace(key, value);
            }
            if (!aliases_map.contains(filename_stem))
            {
                aliases_map.emplace(filename_stem, aliases_map.size());
            }
            aliases[aliases_map.at(filename_stem)] = alias;
        }
        else if (entry.is_directory())
        {
            aliasFmt alias{};
            for (const auto& entry_final : std::filesystem::directory_iterator(current_path))
            {
                const auto& final_path = entry_final.path();
                if (!entry_final.is_regular_file() || final_path.filename().stem() != "global")
                    continue;
                auto current_json = json::parse(std::ifstream{final_path});
                auto filename_stem = current_path.filename().stem().string();
                // TODO character name parsing
                for (auto& [key, value] : current_json.items())
                {
                    if (!value.is_string())
                        throw std::runtime_error{"Invalid alias name"};
                    alias[key] = value;
                }
                if (!aliases_map.contains(filename_stem))
                {
                    aliases_map.emplace(filename_stem, aliases_map.size());
                }
                aliases[aliases_map.at(filename_stem)] = alias;
            }
        }
    }
}

void Parser::loadCommands()
{
    auto command_json = json::parse(std::ifstream{path_db / "command_db.json"});
    for (auto& [cmd_id_json, cmd_data] : command_json.items())
    {
        int cmd_id = std::stoi(cmd_id_json);
        std::vector<ArgFormat> formats;
        int size = 0;
        if (cmd_data.contains("format"))
        {
            formats = fmt_parse(cmd_data["format"]);
            for (auto [length, format_type] : formats)
                size += length;
        }
        else if (cmd_data.contains("size"))
        {
            size = cmd_data["size"];
            const FormatDef fmt{FormatDef::string};
            formats.emplace_back(size, fmt);
        }
        else
            throw std::runtime_error{"No format or size given for " + cmd_id_json};
        const auto name = cmd_data.contains("name") ? std::optional<std::string>(cmd_data["name"]) : std::nullopt;

        auto parseAlias = [&]()
        {
            std::unordered_map<int, int> id_aliases{};
            if (!cmd_data.contains("alias"))
                return id_aliases;
            if (const auto alias_params = cmd_data["alias"]; alias_params.is_string())
            {
                const std::string alias_str = alias_params;
                if (!aliases_map.contains(alias_str))
                    return id_aliases;
                auto alias_id = aliases_map[alias_str];
                for (int i = 0; i < formats.size(); ++i)
                {
                    id_aliases.emplace(i, alias_id);
                }
            }
            else if (alias_params.is_object())
            {
                for (auto& [id, alias_name] : alias_params.items())
                {
                    int i = std::stoi(id);
                    const std::string alias_str = alias_name;
                    if (!aliases_map.contains(alias_str))
                        continue;
                    auto alias_id = aliases_map[alias_str];
                    id_aliases.emplace(i, alias_id);
                }
            }
            return id_aliases;
        };
        auto id = Id{cmd_id, cmd_id_json, formats, parseAlias(), name, size};
        cmd_id_db.emplace(cmd_id, id);
    }
}

Parser::Parser(const std::string& game)
{
    this->game = game;
    {
        // We get the path
        int dirname_length;
        const int path_len = wai_getExecutablePath(nullptr, 0, nullptr);
        const auto path_c = static_cast<char*>(malloc(path_len + 1));
        wai_getExecutablePath(path_c, path_len, &dirname_length);
        path_c[dirname_length] = '\0';
        path_db = static_cast<std::string>(path_c);
        path_db.append("static_db").append(game);
        free(path_c);
    }
    loadAliases();
    loadCommands();
    auto function_json = json::parse(std::ifstream{path_db / "function_db.json"});
    for (auto& [key, value] : function_json.items())
    {
        // TODO function separation
        int cmd_id = std::stoi(key);
        if (!value.contains("end"))
        {
            continue;
        }
        int end_id = value["end"];
    }
}

std::vector<Command> Parser::register_file(const std::string& in)
{
    std::ifstream file{in.c_str(), std::ios::in | std::ios::binary};
    if (!file)
    {
        throw std::istream::failure("Could not open file");
    }
    std::vector<char> bytes;
    bytes.reserve(256);
    auto read_file_int = [&file]()
    {
        char tmp[4];
        file.read(tmp, 4);
        return *reinterpret_cast<int*>(tmp);
    };
    auto read_file = [&file](Id& id)
    {
        // We will assume little endian because there is no way Arcsys has/uses processors with big endian
        std::vector<std::string> arguments;
        for (auto [length_total, format] : id.formats)
        {
            if (format.type == FormatDef::empty)
                return std::vector<std::string>{""};
            std::vector<char> tmp(length_total);
            file.read(tmp.data(), length_total);
            switch (format.type)
            {
            case FormatDef::string:
                arguments.emplace_back(tmp.data());
                break;
            case FormatDef::integer:
                arguments.emplace_back(std::to_string(*reinterpret_cast<int*>(tmp.data())));
                break;
            case FormatDef::u_integer:
                arguments.emplace_back(std::to_string(*reinterpret_cast<unsigned int*>(tmp.data())));
                break;
            default:
                throw std::runtime_error("Unsupported format");
            }
        }
        return arguments;
    };
    file.seekg(0, std::ios::end);
    int end = file.tellg();
    file.seekg(0, std::ios::beg);
    const int FUNCTION_COUNT = read_file_int();
    file.seekg(4 + 0x24 * FUNCTION_COUNT);

    std::vector<Command> commands;
    while (file.tellg() < end)
    {
        Id& id = cmd_id_db.at(read_file_int());
        commands.emplace_back(id, read_file(id));
        // TODO don't forget to remove this ig
        auto var = debug_parser(commands.back().arguments);
    }
    return commands;
}
