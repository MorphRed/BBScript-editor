//
// Created by morphred on 9/24/25.
//

#include "Editor.h"


Editor::Editor(Parser& parser) : parser(parser)
{
}

void Editor::parse(std::string& in)
{
    piece_table = PieceTable{parser.register_file(in)};
}

void Editor::insert(const int line, std::vector<Command>& commands)
{
    piece_table.insert(line, commands);
}

std::vector<std::vector<std::string>> Editor::getText(const int pos, const int length)
{
    return piece_table.getText(pos, length);
};

std::vector<std::string> Editor::getText(const int pos)
{
    return piece_table.getText(pos);
}

