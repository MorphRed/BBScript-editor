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

void Editor::insert(const int index, const std::vector<Command>& commands)
{
    piece_table.insert(index, commands);
}

std::vector<std::vector<std::string>> Editor::getText(const int pos, const int length)
{
    std::vector<std::vector<std::string>> output;
    for (const auto command : piece_table.getCommand(pos, length))
    {
        auto& id = command->id;
        std::vector<std::string> str_cmd;
        str_cmd.emplace_back(id.name.value_or(id.str_id));
        if (!id.aliases.empty())
        {
            for (int i = 0; i < command->arguments.size(); i++)
            {
                const auto& arg = command->arguments[i];
                if (id.aliases.contains(i) && parser.aliases.at(id.aliases[i]).contains(arg))
                {
                    str_cmd.emplace_back(parser.aliases[id.aliases[i]][arg]);
                }
                else
                {
                    str_cmd.emplace_back(arg);
                }
            }
        }
        else
        {
            str_cmd.insert(str_cmd.end(), command->arguments.begin(), command->arguments.end());
        }
        output.emplace_back(str_cmd);
    }
    return output;
}

std::vector<std::vector<std::string>> Editor::getText(const int pos)
{
    return getText(pos, 1);
}

