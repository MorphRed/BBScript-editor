//
// Created by morphred on 9/11/25.
//

#include "Command.h"

#include <algorithm>

Command::Command(const Id& id, std::vector<char>& byte_arguments) : id(id), byte_arguments(std::move(byte_arguments))
{
}

std::optional<std::vector<std::string>> Command::getText()
{
    if (byte_arguments.empty())
        return std::nullopt;
    std::vector<std::string> texts;
    std::size_t index = 0;
    for (auto [length_total, format_type] : id.format)
    {
        texts.emplace_back(&byte_arguments[index], length_total);
        index += length_total;
    }
    return texts;
}




