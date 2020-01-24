#include <iostream>
#include <string>
#include <vector>
#include "kernel.h"

int main(int argc, char** argv) {
    int substring_count = 0;
    std::string substrings;
    std::string query;

    std::cin >> substring_count;
    for (int i = 0; i < substring_count; i++) {
        std::string curr_substring;
        std::cin >> curr_substring;
        substrings += curr_substring + '%';
    }

    std::cin >> query;
    query += '%';

    bool fallback = false;
    std::vector<char> substrings_buffer(substrings.begin(), substrings.end());
    std::vector<char> query_buffer(query.begin(), query.end());
    int worst_case_matches = query.size() * substring_count;
    std::vector<int> substring_indexes(worst_case_matches);
    std::vector<int> query_indexes(worst_case_matches);
    int substring_size = substrings.size();

    AhoCorasick_search(&substring_size,
                substrings_buffer.data(), query_buffer.data(),
                substring_indexes.data(), query_indexes.data(),
                &fallback);

    for (int i = 0; ; i++) {
        if (substring_indexes[i] == -1) break;
        for (int j = substring_indexes[i]; substrings[j] != '%'; j++) {
            std::cout << substrings[j];
        }
        std::cout << ":" << query_indexes[i] << std::endl;
    }

    return 0;
}

