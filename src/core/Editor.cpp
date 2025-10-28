//
// Created by morphred on 9/24/25.
//

#include "Editor.h"


Editor::Editor(Parser& parser) : parser(parser)
{
}

void Editor::parse(const std::string& in)
{
    piece_table = PieceTable{parser.register_file(in)};
}

void Editor::insert(const int index, std::vector<Command>& commands)
{
    piece_table.insert(index, commands);
}

std::vector<std::vector<std::string>> Editor::getText(const int pos, const int length)
{
    std::vector<std::vector<std::string>> output;
    for (const auto command : piece_table.getCommand(pos, length))
    {
        std::vector<std::string> commands;
        commands.emplace_back(command->id.name.value_or(command->id.str_id));
        commands.insert(commands.end(), command->arguments.begin(), command->arguments.end());
        output.emplace_back(commands);
    }
    return output;
}

std::vector<std::vector<std::string>> Editor::getText(const int pos)
{
    return getText(pos, 1);
}

