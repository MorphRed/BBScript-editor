//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_BLOCK_H
#define BBSCRIPT_EDITOR_BLOCK_H

#include <unordered_map>
#include <vector>
#include "Command.h"

std::string debug_parser(const std::vector<std::string>& arguments);
std::string debug_parser(const std::vector<std::vector<std::string>>& arguments);

class Parser
{
    std::string game;
    std::unordered_map<std::string, std::unordered_map<int, std::string>> aliases{};
    static std::vector<ArgFormat> fmt_parse(const std::string& fmt_txt);

public:
    std::unordered_map<int, Id> cmd_id_db{};
    explicit Parser(const std::string& game);
    std::vector<Command> register_file(const std::string& in);
};

#endif //BBSCRIPT_EDITOR_BLOCK_H
