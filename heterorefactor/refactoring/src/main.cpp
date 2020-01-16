#include "exclusion_finder.h"
#include "type_transformer.h"
#include "transform_propagator.h"
#include "access_transformer.h"
#include "allocate_transformer.h"
#include "recursion_finder.h"
#include "localvar_packer.h"

#include <cassert>
#include <vector>
#include <string>

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
    bool perform_fp = false;
    bool perform_int = false;
    std::string output_file = "";
};

Sawyer::CommandLine::SwitchGroup commandline_switches(Settings &settings) {
    using namespace Sawyer::CommandLine;
    SwitchGroup switches;
    switches.doc("These switches control the HeteroRefactor tool.");
    switches.insert(Switch("rec").intrinsicValue(true, settings.perform_rec)
            .doc("Enable the recursive data structures refactoring."));
    switches.insert(Switch("fp").intrinsicValue(true, settings.perform_fp)
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
            .doc("synopsis", "@prop{programName} @v{switches} -u @v{output} @v{input}")
            .with(Rose::CommandLine::genericSwitches())
            .with(commandline_switches(settings))
            .parse(argvlist).apply().unparsedArgs(true);
}


int main(int argc, char *argv[]) {
    ROSE_INITIALIZE;

    Settings settings;
    std::vector<std::string> argvlist(argv, argv+argc);
    argvlist = commandline_processing(argvlist, settings);

    argvlist.push_back("-std=c++11");
    if (!(settings.perform_rec ||
                settings.perform_fp ||
                settings.perform_int)) {
        INFO_IF(true, "Please specify transformations to be performed.\n");
    }
    if (settings.output_file != "") {
        argvlist.push_back("-rose:skipfinalCompileStep");
        argvlist.push_back("-rose:o");
        argvlist.push_back(settings.output_file);
    }

    auto project = frontend(argvlist);
    AstTests::runAllTests(project);

    AccessTransformer acc_trans(project);
    AllocateTransformer alloc_trans(project);
    acc_trans.collect_access();
    alloc_trans.collect_types();

    ExclusionFinder ex_finder(project);
    ex_finder.run();
    auto excluded = ex_finder.get_excluded();
    auto addressof_propagated =
        ex_finder.get_addressof_propagated();

    TypeTransformer type_trans(project);
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
    LocalVarPacker packer(project);
    packer.set_target(&target);
    packer.run();

    AstTests::runAllTests(project);
    return backend(project);
}
