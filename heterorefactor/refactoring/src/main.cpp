#include "exclusion_finder.h"
#include "type_transformer.h"
#include "transform_propagator.h"
#include "access_transformer.h"
#include "allocate_transformer.h"
#include "recursion_finder.h"
#include "localvar_packer.h"

#include <cassert>

int main(int argc, char *argv[]) {
    ROSE_INITIALIZE;
    auto project = frontend(argc, argv);
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
