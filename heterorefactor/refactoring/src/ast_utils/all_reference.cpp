#include <ast_utils.h>
#include <misc_utils.h>

namespace ast_utils {
    std::set<SgNode *> all_reference(
            SgProject* project, std::set<SgNode *> &seed) {
        std::set<SgNode *> references;

        for (auto var_ref : NodeQuery::querySubTree(
                project, V_SgVarRefExp)) {
            auto var_ref_v = isSgVarRefExp(var_ref);
            auto var = var_ref_v->get_symbol()->get_declaration();
            if (misc_utils::insideSystemHeader(var_ref_v)) continue;
            if (seed.count(var)) references.insert(var_ref);
        }

        for (auto func_ref : NodeQuery::querySubTree(
                project, V_SgFunctionRefExp)) {
            auto func_ref_v = isSgFunctionRefExp(func_ref);
            auto func = func_ref_v->get_symbol()->
                get_declaration()->get_definingDeclaration();
            if (misc_utils::insideSystemHeader(func_ref_v)) continue;
            if (seed.count(func)) references.insert(func_ref);
        }

        return references;
    }
}
