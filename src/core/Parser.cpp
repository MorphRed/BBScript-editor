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

std::vector<ArgFormat> Parser::fmt_parse(const std::string& fmt)
{
    std::vector<ArgFormat> format;
    if (fmt.empty())
    {
        format.push_back({0, &ArgFormat::empty});
        return format;
    }
    int fmt_size;
    int start = 0;
    for (int i = 0; i < fmt.size(); i++)
        if (std::isalpha(fmt[i]))
        {
            if (start == i)
                fmt_size = 1;
            else
                std::from_chars(&fmt[start], &fmt[i], fmt_size);
            start = i + 1;
            if (fmt[i] == 's')
                format.push_back({fmt_size, &ArgFormat::string});
            else
            {
                FormatDef* arg_format;
                switch (fmt[i])
                {
                case 'i':
                    arg_format = &ArgFormat::integer;
                    break;
                case 'I':
                    arg_format = &ArgFormat::u_integer;
                    break;
                case 'b':
                    arg_format = &ArgFormat::byte;
                    break;
                case 'B':
                    arg_format = &ArgFormat::u_byte;
                    break;
                default:
                    throw std::runtime_error("Invalid format");
                }
                for (int j = 0; j < fmt_size; j++)
                    format.push_back({arg_format->size, arg_format});
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
            formats = {{size, &ArgFormat::string}};
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

void Parser::register_file(std::ifstream& file)
{
    std::vector<char> bytes{};
    bytes.reserve(256);
    auto read_file_int = [&file]()
    {
        char tmp[4];
        file.read(tmp, 4);
        return *reinterpret_cast<int*>(tmp);
    };
    auto read_file = [&bytes, &file](const int size)
    {
        // We will assume little endian because there is no way Arcsys has/uses processors with big endian
        bytes.clear();
        bytes.resize(size);
        file.read(bytes.data(), size);
    };
    file.seekg(0, std::ios::beg);
    const int FUNCTION_COUNT = read_file_int();
    file.seekg(4 + 0x24 * FUNCTION_COUNT);
    std::vector<Command> commands{};
    while (!file.eof())
    {
        const int cmd_id = read_file_int();
        std::vector<char> byte_arguments;
        Id& id = cmd_id_db.at(cmd_id);
        read_file(id.size);
        byte_arguments = bytes;
        commands.emplace_back(id, byte_arguments);
    }
    editors.emplace_back(commands);
}
