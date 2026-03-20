//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_BLOCK_H
#define BBSCRIPT_EDITOR_BLOCK_H

#include <filesystem>
#include <unordered_map>
#include <vector>
#include "Command.h"

std::string debug_parser(const std::vector<std::string>& arguments);
std::string debug_parser(const std::vector<std::vector<std::string>>& arguments);

class Parser
{
    std::string game;
    std::filesystem::path path_db;
    static std::vector<ArgFormat> fmt_parse(const std::string& fmt_txt);

public:
    using aliasFmt = std::unordered_map<std::string, std::string>;
    std::unordered_map<int, Id> cmd_id_db{};
    // Use a string since commands end up being stored as strings
    std::unordered_map<int, aliasFmt> aliases{};
    std::unordered_map<std::string, int> aliases_map{};
    explicit Parser(const std::string& game);
    std::vector<Command> register_file(const std::string& in);
    void loadCommands();
    void loadAliases();
};

#endif //BBSCRIPT_EDITOR_BLOCK_H
