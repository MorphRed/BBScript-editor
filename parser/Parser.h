//
// Created by morphred on 9/11/25.
//

#ifndef BBSCRIPT_EDITOR_BLOCK_H
#define BBSCRIPT_EDITOR_BLOCK_H
#include <fstream>
#include <unordered_map>
#include <vector>
#include "Command.h"
#include "include/nlohmann/json.hpp"

class Parser
{
    std::ifstream& file;
    std::unordered_map<std::string, std::unordered_map<int, std::string>> aliases{};
    std::unordered_map<int, Id> cmd_id_db{};
    std::vector<Command> commands{};
    std::vector<int> function_index{};
    std::vector<char> bytes{};
    void read(int size);
    int read_int() const;
    static std::vector<Format> fmt_parse(const std::string& fmt);
public:
    Parser(std::ifstream& file, const std::string& game);
    void decode();
};

#endif //BBSCRIPT_EDITOR_BLOCK_H
