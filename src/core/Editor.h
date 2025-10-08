//
// Created by morphred on 9/24/25.
//

#ifndef BBSCRIPT_EDITOR_EDITOR_H
#define BBSCRIPT_EDITOR_EDITOR_H

#include "Command.h"
#include "Parser.h"
#include "PieceTable.h"

class Editor
{
    Parser& parser;
    PieceTable piece_table;

public:
    Editor(Parser& parser, const std::vector<Command>& commands);
    void insert(int line, std::vector<Command>& commands);
};


#endif //BBSCRIPT_EDITOR_EDITOR_H
