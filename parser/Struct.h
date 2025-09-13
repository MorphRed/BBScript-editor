//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_CHUNK_H
#define BBSCRIPT_EDITOR_CHUNK_H

#include <optional>
#include <string>
#include <vector>

enum class FormatType
{
    _ = -1,
    s = 0,
    i = 1,
    I = 2,
    b = 3,
    B = 4
};

typedef struct
{
    const FormatType format;
    const int size;
} FormatDef;

typedef struct
{
    int length;
    FormatDef* format_type;
} Format;

typedef struct
{
    int cmd_id;
    std::string str_id;
    std::vector<Format> formats;
    std::optional<std::string> name;
    int size;
} Id;


#endif //BBSCRIPT_EDITOR_CHUNK_H
