//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_COMMAND_H
#define BBSCRIPT_EDITOR_COMMAND_H

#include <vector>

#include "Struct.h"

class Command
{
    Id* id;
    std::vector<char> byte_arguments;

public:
    Command(Id* id, const std::vector<char>& byte_arguments);
};


#endif //BBSCRIPT_EDITOR_COMMAND_H
