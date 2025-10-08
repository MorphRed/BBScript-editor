//
// Created by morphred on 9/24/25.
//

#include "Editor.h"


Editor::Editor(Parser& parser, const std::vector<Command>& commands) : parser(parser), piece_table(commands)
{
}

void Editor::insert(const int line, std::vector<Command>& commands)
{
    piece_table.insert(line, commands);
}

