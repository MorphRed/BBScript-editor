//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_CHUNK_H
#define BBSCRIPT_EDITOR_CHUNK_H

#include <optional>
#include <string>
#include <vector>

typedef struct
{
    const int size;
} FormatDef;

typedef struct ArgFormat
{
    int length_total;
    FormatDef* format_type;
    static FormatDef empty;
    static FormatDef string;
    static FormatDef integer;
    static FormatDef u_integer;
    static FormatDef byte;
    static FormatDef u_byte;
} ArgFormat;

typedef struct
{
    int cmd_id;
    std::string str_id;
    std::vector<ArgFormat> format;
    std::optional<std::string> name;
    int size;
} Id;


#endif //BBSCRIPT_EDITOR_CHUNK_H
