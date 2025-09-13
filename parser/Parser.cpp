//
// Created by morphred on 9/11/25.
//

#include "Parser.h"

#include <charconv>
#include <format>
#include <iostream>
#include <qtextstream.h>
#include <vector>

#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

static FormatDef empty{FormatType::_, 0};
static FormatDef string{FormatType::s, 1};
static FormatDef integer{FormatType::i, 4};
static FormatDef u_integer{FormatType::I, 4};
static FormatDef byte{FormatType::b, 1};
static FormatDef u_byte{FormatType::B, 1};

std::vector<Format> Parser::fmt_parse(const std::string& fmt)
{
    std::vector<Format> formats;
    if (fmt.empty())
    {
        formats.push_back({0, &empty});
        return formats;
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
                formats.push_back({fmt_size, &string});
            else
            {
                FormatDef* format;
                switch (fmt[i])
                {
                case 'i':
                    format = &integer;
                    break;
                case 'I':
                    format = &u_integer;
                    break;
                case 'b':
                    format = &byte;
                    break;
                case 'B':
                    format = &u_byte;
                    break;
                default:
                    throw std::runtime_error("Invalid format");
                }
                for (int j = 0; j < fmt_size; j++)
                    formats.push_back({1, format});
            }
        }
    return formats;
}

Parser::Parser(std::ifstream& file, const std::string& game) : file(file)
{
    bytes.reserve(256);
    const std::string path{"../static_db/" + game + "/"};
    auto command_db = json::parse(std::ifstream{path + "command_db.json"});
    auto function_db = json::parse(std::ifstream{path + "function_db.json"});
    for (auto& [key, value] : command_db.items())
    {
        int cmd_id;
        std::from_chars(key.data(), key.data() + key.size(), cmd_id);
        std::vector<Format> formats;
        int size = 0;
        if (value.contains("format"))
        {
            std::string str_fmt;
            str_fmt = value["format"];
            formats = fmt_parse(str_fmt);
            for (auto [length, format_type] : formats)
                size += length * format_type->size;
        }
        else if (value.contains("size"))
        {
            size = value["size"];
            formats = {{size, &string}};
        }
        else
            throw std::runtime_error{"No format or given for " + key};
        auto id = (value.contains("name"))
                      ? Id{cmd_id, key, formats, value["name"], size}
                      : Id{cmd_id, key, formats, std::nullopt, size};
        cmd_id_db.insert({cmd_id, id});
    }
    // for (auto& [key, value] : function_db.items())
    // {
    //     int cmd_id;
    //     std::from_chars(key.data(), key.data() + key.size(), cmd_id);
    //     value
    // }
    // TODO alias files
}

void Parser::read(const int size)
{
    // We will assume little endian because there is no way Arcsys has/uses processors with big endian
    bytes.clear();
    bytes.resize(size);
    file.read(bytes.data(), size);
}

int Parser::read_int() const
{
    char tmp[4];
    file.read(tmp, 4);
    return *reinterpret_cast<int*>(tmp);
}

void Parser::decode()
{
    file.seekg(0, std::ios::beg);
    const int FUNCTION_COUNT = read_int();
    file.seekg(4 + 0x24 * FUNCTION_COUNT);
    while (!file.eof())
    {
        const int cmd_id = read_int();
        std::vector<char> byte_arguments;
        Id* id = &cmd_id_db.at(cmd_id);
        read(id->size);
        byte_arguments = bytes;
        commands.emplace_back(id, byte_arguments);
    }
}
