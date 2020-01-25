/*===============================================================*/
/*                                                               */
/*                           utils.h                             */
/*                                                               */
/*                       Utility functions                       */
/*                                                               */
/*===============================================================*/

#include "ap_int.h"
void print_usage(char* filename);

void parse_sdaccel_command_line_args(
    int argc,
    char** argv,
    std::string& kernelFile,
    std::string& outFile);

void parse_sdsoc_command_line_args(
    int argc,
    char** argv,
    std::string& outFile);

