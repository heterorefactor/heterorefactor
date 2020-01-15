#include "recursion_finder.h"

RecursionFinder::RecursionFinder(SgProject* project) {
    m_project = project;
}

std::set<SgFunctionDeclaration *>
RecursionFinder::get_recursion_functions(void) {
    return m_recursion;
}

static bool is_recursion(SgFunctionDeclaration *func);

void RecursionFinder::run(void) {
    auto defining = ast_utils::all_used_func_def(m_project);
    std::copy_if(defining.begin(), defining.end(),
            std::inserter(m_recursion, m_recursion.end()),
            is_recursion);
}

static bool is_recursion(SgFunctionDeclaration *func) {
    for (auto i: NodeQuery::querySubTree(func, V_SgFunctionRefExp)) {
        auto ref = isSgFunctionRefExp(i);
        auto ref_func = ref->getAssociatedFunctionDeclaration();
        auto def_ref_func = ref_func->get_definingDeclaration();

        INFO_IF(def_ref_func == func, "[finder] Found recursion: ") <<
            misc_utils::debug_info(func) << std::endl;
        if (def_ref_func == func) return true;
    }

    return false;
}
