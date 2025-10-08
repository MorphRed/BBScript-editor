//
// Created by morphred on 9/24/25.
//

#include "Editor.h"

#include <qelapsedtimer.h>

PieceTable::PieceTable(const std::vector<Command>& commands)
{
    buffers = {{commands}};
    
    root = new Node(0, 0, commands.size());
}

Editor::Editor(const std::vector<Command>& commands) : piece_table(commands)
{
}

