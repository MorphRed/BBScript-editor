//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_CHUNK_H
#define BBSCRIPT_EDITOR_CHUNK_H

#include <optional>
#include <string>
#include <vector>

typedef struct FormatDef
{
    const int size;
    static FormatDef empty;
    static FormatDef string;
    static FormatDef integer;
    static FormatDef u_integer;
    static FormatDef byte;
    static FormatDef u_byte;
} FormatDef;

typedef struct
{
    int length_total;
    FormatDef* format_type;
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
