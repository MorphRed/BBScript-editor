//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_BLOCK_H
#define BBSCRIPT_EDITOR_BLOCK_H
#include <fstream>
#include <unordered_map>
#include <vector>
#include "Command.h"
#include "Editor.h"


class Parser
{
    std::string game;
    std::unordered_map<std::string, std::unordered_map<int, std::string>> aliases{};
    std::unordered_map<int, Id> cmd_id_db{};
    static std::vector<Format> fmt_parse(const std::string& fmt);

public:
    explicit Parser(const std::string& game);
    Editor register_file(std::ifstream& file);
};

#endif //BBSCRIPT_EDITOR_BLOCK_H
