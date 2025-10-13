//
// Created by morphred on 9/11/25.
//

#include "Command.h"

#include <algorithm>
#include <assert.h>
#include <stdexcept>

Command::Command(const Id& id, const std::vector<char>& byte_arguments) : id(id)
{
    assert(id.size == byte_arguments.size());
    this->byte_arguments = byte_arguments;
}

std::vector<std::string> Command::getText()
{
    std::vector<std::string> texts;
    if (id.name != std::nullopt)
        texts.push_back(id.name.value());
    else
        texts.push_back(id.str_id);
    if (byte_arguments.empty())
        return texts;
    std::size_t index = 0;
    for (auto [length_total, format_type] : id.format)
    {
        // Switch case not possible because it's a pointer
        if (format_type == &FormatDef::integer)
            texts.push_back(std::to_string(*reinterpret_cast<int*>(&byte_arguments[index])));
        else if (format_type == &FormatDef::u_integer)
            texts.push_back(std::to_string(*reinterpret_cast<unsigned int*>(&byte_arguments[index])));
        else if (format_type == &FormatDef::string)
            texts.emplace_back(&byte_arguments[index]);
        else
            throw std::runtime_error("Unsupported format");
        index += length_total;
    }
    return texts;
}