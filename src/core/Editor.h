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
    explicit Editor(Parser& parser);
    void parse(std::string& in);
    void insert(int line, std::vector<Command>& commands);
    std::vector<std::vector<std::string>> getText(int pos, int length);
    std::vector<std::string> getText(int pos);
};


#endif //BBSCRIPT_EDITOR_EDITOR_H
