//
// Created by morphred on 9/11/25.
//

#include "Parser.h"

#include <charconv>
#include <format>
#include <iostream>
#include <vector>

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

Parser::Parser(const std::string& game)
{
    this->game = game;
    int dirname_length, path_len;
    path_len = wai_getExecutablePath(nullptr, 0, nullptr);
    auto path_c = static_cast<char*>(malloc(path_len + 1));
    wai_getExecutablePath(path_c, path_len, &dirname_length);
    path_c[dirname_length] = '\0';
    std::string path = static_cast<std::string>(path_c) + "/static_db/" + game + "/";
    free(path_c);

    auto command_json = json::parse(std::ifstream{path + "command_db.json"});
    auto function_json = json::parse(std::ifstream{path + "function_db.json"});
    for (auto& [key, value] : command_json.items())
    {
        int cmd_id;
        std::from_chars(key.data(), key.data() + key.size(), cmd_id);
        std::vector<ArgFormat> formats;
        int size = 0;
        if (value.contains("format"))
        {
            formats = fmt_parse(value["format"]);
            for (auto [length, format_type] : formats)
                size += length;
        }
        else if (value.contains("size"))
        {
            size = value["size"];
            const FormatDef fmt{FormatDef::string};
            formats.emplace_back(size, fmt);
        }
        else
            throw std::runtime_error{"No format or given for " + key};
        auto id = (value.contains("name"))
                      ? Id{cmd_id, key, formats, value["name"], size}
                      : Id{cmd_id, key, formats, std::nullopt, size};
        cmd_id_db.insert({cmd_id, id});
    }
    for (auto& [key, value] : function_json.items())
    {
        // int cmd_id;
        // std::from_chars(key.data(), key.data() + key.size(), cmd_id);
        // value
    }
    // TODO alias files
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
            char tmp[length_total];
            file.read(tmp, sizeof(tmp));
            switch (format.type)
            {
            case FormatDef::string:
                arguments.push_back(tmp);
                break;
            case FormatDef::integer:
                arguments.emplace_back(std::to_string(*reinterpret_cast<int*>(tmp)));
                break;
            case FormatDef::u_integer:
                arguments.emplace_back(std::to_string(*reinterpret_cast<unsigned int*>(tmp)));
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
        auto var = debug_parser(commands.back().arguments);
    }
    return commands;
}
