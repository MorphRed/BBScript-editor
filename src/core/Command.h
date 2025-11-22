//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_COMMAND_H
#define BBSCRIPT_EDITOR_COMMAND_H

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

struct FormatDef
{
    enum Type
    {
        empty,
        string,
        integer,
        u_integer,
        byte,
        u_byte,
    };

    const Type type;
    int size;

    explicit FormatDef(const Type type) : type(type)
    {
        switch (type)
        {
        case empty:
            size = 0;
            break;
        case string:
        case byte:
        case u_byte:
            size = 1;
            break;
        case integer:
        case u_integer:
            size = 4;
            break;
        default:
            throw std::invalid_argument("Implemented format type");
        }
    }
};

struct ArgFormat
{
    int length_total;
    FormatDef format;
};

struct Id
{
    int cmd_id;
    std::string str_id;
    std::vector<ArgFormat> formats;
    std::optional<std::string> name;
    int size;
};

struct Command
{
    Id& id;
    std::vector<std::string> arguments;
};


#endif //BBSCRIPT_EDITOR_COMMAND_H
