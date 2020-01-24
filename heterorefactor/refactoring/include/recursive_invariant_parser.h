#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>

class RecursiveInvariantParser {
public:
    RecursiveInvariantParser(std::string filename);
    std::map<std::string, int> get_type_size_mapping(void);
    std::map<std::string, int> get_func_depth_mapping(void);

protected:
    std::map<std::string, int> m_map_type_size;
    std::map<std::string, int> m_map_func_depth;
};
