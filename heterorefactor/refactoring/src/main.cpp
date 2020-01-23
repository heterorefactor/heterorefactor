#include "exclusion_finder.h"
#include "type_transformer.h"
#include "transform_propagator.h"
#include "access_transformer.h"
#include "allocate_transformer.h"
#include "recursion_finder.h"
#include "localvar_packer.h"
#include "stack_tracker.h"

#include <cassert>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

#include <CommandLine.h>
#include <Sawyer/CommandLine.h>
static const char* purpose =
    "This tool performs refactoring for heterogeneous computing with FPGA.";
static const char* description =
    "This tool refactor heterogeneous computing code in C++. "
    "Currently supported refactoring patterns are: "
    "(1) recursive data structures; "
    "(2) floating point numbers; and "
    "(3) integer bitwidth.";

struct Settings {
    bool perform_rec = false;
    int  perform_fp_fraction_bits = 0;
    bool perform_int = false;
    std::string perform_rec_instrument_file = "";
    std::string output_file = "";
};

Sawyer::CommandLine::SwitchGroup commandline_switches(Settings &settings) {
    using namespace Sawyer::CommandLine;
    SwitchGroup switches("HeteroRefactor switches");
    switches.doc("These switches control the HeteroRefactor tool.");
    switches.insert(Switch("rec").intrinsicValue(true, settings.perform_rec)
            .doc("Enable the recursive data structures refactoring."));
    switches.insert(Switch("instrument")
            .argument("profile_output",
                anyParser(settings.perform_rec_instrument_file))
            .doc("Refactor the program for recursive data structures "
                "instrumentation."));
    switches.insert(Switch("fp")
            .argument("fraction_bits",
                anyParser(settings.perform_fp_fraction_bits))
            .doc("Enable the floating point numbers refactoring."));
    switches.insert(Switch("int").intrinsicValue(true, settings.perform_int)
            .doc("Enable the integer bitwidth refactoring."));
    switches.insert(Switch("output", 'u')
            .argument("string", anyParser(settings.output_file))
            .doc("Output path for refactored program."));
    return switches;
}

std::vector<std::string>
commandline_processing(
        std::vector< std::string > &argvlist,
        Settings &settings) {
    using namespace Sawyer::CommandLine;
    return Rose::CommandLine::createEmptyParserStage(purpose, description)
            .longPrefix("-")
            .doc("synopsis",
                    "@prop{programName} [@v{gcc-switches}] "
                    "[@v{hrf-switches}] -u @v{output} @v{input}")
            .with(Rose::CommandLine::genericSwitches())
            .with(commandline_switches(settings))
            .parse(argvlist).apply().unparsedArgs(true);
}


void add_default_parameters(std::vector<std::string> &argvlist) {
    argvlist.push_back("-std=c++11");
}

void add_libraries(std::vector<std::string> &argvlist, char *path) {
    char buf[PATH_MAX];
    char *res = realpath(path, buf);
    if (res) {
        char *dir = dirname(res);
        argvlist.push_back("-I");
        argvlist.push_back(std::string() + dir +
                "/../../../../libraries/xilinx_include");
        argvlist.push_back("-I");
        argvlist.push_back(std::string() + dir +
                "/../../../../libraries/template-hls-float/include");
    }
}

void add_output_file(std::vector<std::string> &argvlist, std::string output) {
    argvlist.push_back("-rose:skipfinalCompileStep");
    argvlist.push_back("-rose:o");
    argvlist.push_back(output);
}

void prepend_preprocessing_info(std::vector<std::string> &argvlist,
        std::string str) {
    // TODO: we need a more decent way to identify which is the input file
    for (auto i = argvlist.rbegin(); i != argvlist.rend(); i++) {
        struct stat path_stat;
        stat(i->c_str(), &path_stat);
        if (S_ISREG(path_stat.st_mode)) {
            char buf[PATH_MAX];
            char *res = realpath(i->c_str(), buf);
            char *base = basename(res);
            char temp[PATH_MAX] = "/tmp/hetero-XXXXXX-";
            strcat(temp, base);
            if (mkstemps(temp, strlen(base) + 1) >= 0) {
                // TODO: we need to remove this file after exit
                std::ofstream newinput(temp, std::ios::binary);
                newinput << str;
                std::ifstream oldinput(*i, std::ios::binary);
                std::copy(std::istreambuf_iterator<char>(oldinput),
                        std::istreambuf_iterator<char>(),
                        std::ostreambuf_iterator<char>(newinput));
                *i = temp;
            }
            break;
        }
    }
}


int main(int argc, char *argv[]) {
    ROSE_INITIALIZE;

    Settings settings;
    std::vector<std::string> argvlist(argv, argv+argc);
    argvlist = commandline_processing(argvlist, settings);

    if (!(settings.perform_rec ||
                settings.perform_rec_instrument_file == "" ||
                settings.perform_fp_fraction_bits == 0 ||
                settings.perform_int)) {
        // Run all transformations by default
        settings.perform_rec = true;
        settings.perform_fp_fraction_bits = 23;
        settings.perform_int = true;
    }

    if (settings.perform_fp_fraction_bits) {
        prepend_preprocessing_info(argvlist,
                std::string() +
                "// === BEGIN FP SUPPORT LIBRARY ===\n"
                "#include <cstddef>\n"
                "#include \"thls/tops/policy_flopoco.hpp\"\n"
                "typedef thls::policy_flopoco<8," +
                    std::to_string(settings.perform_fp_fraction_bits) +
                "> __fpt_policy_t;\n"
                "typedef __fpt_policy_t::value_t __fpt_t;\n"
                "// === END FP SUPPORT LIBRARY ===\n");
    }

    if (settings.perform_rec ||
            settings.perform_rec_instrument_file != "") {
        prepend_preprocessing_info(argvlist,
                std::string() +
                "// === BEGIN REC INSTRUMENT LIBRARY ===\n"
                "#include <stdio.h>\n"
                "#include <stdlib.h>\n"
                "const char *__dst_filename = \"" +
                    settings.perform_rec_instrument_file
                + "\";\n"
                "unsigned long long __dst_file = 0;\n"
                "// === END REC INSTRUMENT LIBRARY ===\n");
    }

    add_default_parameters(argvlist);
    add_libraries(argvlist, argv[0]);
    if (settings.output_file != "") {
        add_output_file(argvlist, settings.output_file);
    }

    auto project = frontend(argvlist);
    if (project->numberOfFiles() != 1) {
        INFO_IF(true, "Please run with -h to see the usages.\n");
        return 0;
    }

    AstTests::runAllTests(project);

    if (settings.perform_int) {
        INFO_IF(true, "Refactoring for integer is to be implemented\n");
    }

    if (settings.perform_fp_fraction_bits) {
        ExclusionFinder ex_finder(project,
                misc_utils::RefactorType::fp);
        ex_finder.run();
        auto excluded = ex_finder.get_excluded();
        auto addressof_propagated =
            ex_finder.get_addressof_propagated();

        TypeTransformer type_trans(project,
                misc_utils::RefactorType::fp);
        type_trans.set_exclusion(&excluded);
        type_trans.transform();

        TransformPropagator prop(project);
        prop.set_addressof_propagated(
                &addressof_propagated);
        prop.propagate();
    }

    if (settings.perform_rec ||
            settings.perform_rec_instrument_file != "") {
        AccessTransformer acc_trans(project);
        AllocateTransformer alloc_trans(project);

        if (settings.perform_rec_instrument_file != "") {
            acc_trans.set_is_instrument();
        }

        acc_trans.collect_access();
        alloc_trans.collect_types();

        ExclusionFinder ex_finder(project,
                misc_utils::RefactorType::rec);
        ex_finder.run();
        auto excluded = ex_finder.get_excluded();
        auto addressof_propagated =
            ex_finder.get_addressof_propagated();

        TypeTransformer type_trans(project,
                misc_utils::RefactorType::rec);
        type_trans.set_exclusion(&excluded);
        type_trans.transform();

        TransformPropagator prop(project);
        prop.set_addressof_propagated(
                &addressof_propagated);
        prop.propagate();

        acc_trans.set_type_transformer(&type_trans);
        acc_trans.transform();

        alloc_trans.set_type_transformer(&type_trans);
        alloc_trans.set_access_transformer(&acc_trans);
        alloc_trans.transform();

        RecursionFinder rec_finder(project);
        rec_finder.run();

        auto target = rec_finder.get_recursion_functions();
        if (settings.perform_rec_instrument_file != "") {
            StackTracker tracker(project);
            tracker.set_target(&target);
            tracker.run();
        } else {
            LocalVarPacker packer(project);
            packer.set_target(&target);
            packer.run();
        }
    }

    AstTests::runAllTests(project);
    return backend(project);
}
