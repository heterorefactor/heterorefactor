#pragma once

extern "C" {
void process_top(
        int *substring_length, char *substrings, char *query,
        int *substring_indexes, int *query_indexes, int *fallback);
};
