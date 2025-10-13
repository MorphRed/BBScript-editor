//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_COMMAND_H
#define BBSCRIPT_EDITOR_COMMAND_H

#include <vector>

#include "Struct.h"

class Command
{
    const Id& id;
    std::vector<char> byte_arguments;

public:
    Command(const Id& id, const std::vector<char>& byte_arguments);
    std::vector<std::string> getText();
};


#endif //BBSCRIPT_EDITOR_COMMAND_H
