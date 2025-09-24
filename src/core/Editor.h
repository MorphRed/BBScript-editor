//
// Created by morphred on 9/24/25.
//

#ifndef BBSCRIPT_EDITOR_EDITOR_H
#define BBSCRIPT_EDITOR_EDITOR_H

#include "Command.h"
#include "red-black-tree/RBTree.h"

class PieceTable : public RBTree
{
    class Buffer
    {
    public:
        std::vector<Command> commands;
    };
    std::vector<Buffer> buffers;
public:
    explicit PieceTable(const std::vector<Command>& commands);
};

class Editor
{
    PieceTable piece_table;

public:
    explicit Editor(const std::vector<Command>& commands);
};


#endif //BBSCRIPT_EDITOR_EDITOR_H
