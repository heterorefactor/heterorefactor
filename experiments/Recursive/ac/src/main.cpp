#include <iostream>
#include <string>
#include <vector>
#include "kernel.h"

#include "CLWorld.h"
#include "CLKernel.h"
#include "CLMemObj.h"
using namespace rosetta;

int main(int argc, char** argv) {
    std::string kernelFile(argv[1]);

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

    std::vector<char> substrings_buffer(substrings.begin(), substrings.end());
    std::vector<char> query_buffer(query.begin(), query.end());

    int worst_case_matches = query.size() * substring_count;
    std::vector<int> substring_indexes(worst_case_matches);
    std::vector<int> query_indexes(worst_case_matches);

    bool fallback = false;

    CLWorld world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);
    world.addProgram(kernelFile);

    CLKernel Kernel(world.getContext(), world.getProgram(),
            "AhoCorasick_search", world.getDevice());

    int substrings_size = substrings.size();
    CLMemObj substrings_size_mem((void*)&substrings_size, sizeof(int), 1, CL_MEM_READ_ONLY);
    CLMemObj substrings_buffer_mem((void*)substrings_buffer.data(), sizeof(char),
            substrings_buffer.size(), CL_MEM_READ_ONLY);
    CLMemObj query_buffer_mem((void*)query_buffer.data(), sizeof(char),
            query_buffer.size(), CL_MEM_READ_ONLY);
    CLMemObj substring_indexes_mem((void*)substring_indexes.data(), sizeof(int),
            substring_indexes.size(), CL_MEM_WRITE_ONLY);
    CLMemObj query_indexes_mem((void*)query_indexes.data(), sizeof(int),
            query_indexes.size(), CL_MEM_WRITE_ONLY);
    CLMemObj fallback_mem((void*)&fallback, sizeof(bool), 1, CL_MEM_WRITE_ONLY);

    world.addMemObj(substrings_size_mem);
    world.addMemObj(substrings_buffer_mem);
    world.addMemObj(query_buffer_mem);
    world.addMemObj(substring_indexes_mem);
    world.addMemObj(query_indexes_mem);
    world.addMemObj(fallback_mem);

    int global_size[3] = {1, 1, 1};
    int local_size[3] = {1, 1, 1};
    Kernel.set_global(global_size);
    Kernel.set_local(local_size);
    world.addKernel(Kernel);

    world.setMemKernelArg(0, 0, 0);
    world.setMemKernelArg(0, 1, 1);
    world.setMemKernelArg(0, 2, 2);
    world.setMemKernelArg(0, 3, 3);
    world.setMemKernelArg(0, 4, 4);
    world.setMemKernelArg(0, 5, 5);

    world.runKernels();

    world.readMemObj(5);

    if (fallback) {
        std::cout << "INFO: falling back!" << std::endl;
        AhoCorasick_search(&substrings_size,
                substrings_buffer.data(), query_buffer.data(),
                substring_indexes.data(), query_indexes.data(),
                &fallback);
    } else {
        world.readMemObj(3);
        world.readMemObj(4);
    }

    world.releaseWorld();

    for (int i = 0; ; i++) {
        if (substring_indexes[i] == -1) break;
        for (int j = substring_indexes[i]; substrings[j] != '%'; j++) {
            std::cout << substrings[j];
        }
        std::cout << ":" << query_indexes[i] << std::endl;
    }

    return 0;
}

