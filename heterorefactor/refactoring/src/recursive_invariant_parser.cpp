#include "recursive_invariant_parser.h"

#include <sstream>

RecursiveInvariantParser::RecursiveInvariantParser(std::string filename) {
    if (filename == "") return;
    std::ifstream inp_file(filename);
    std::string s;
    while (std::getline(inp_file, s)) {
        std::istringstream line(s);
        std::string type, name;
        int size;
        line >> type >> name >> size;
        if (type == "mem") {
            m_map_type_size[name] = size;
        } else if (type == "rec") {
            m_map_func_depth[name] = size;
        }
    }
}

std::map<std::string, int>
RecursiveInvariantParser::get_type_size_mapping(void) {
    return m_map_type_size;
}

std::map<std::string, int>
RecursiveInvariantParser::get_func_depth_mapping(void) {
    return m_map_func_depth;
}
