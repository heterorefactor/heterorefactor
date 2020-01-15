#include "ast_utils.h"
#include "misc_utils.h"

namespace ast_utils {
    std::set<SgInitializedName *> all_used_var_decl(SgProject* project) {
        std::set<SgInitializedName *> vars_used;

        // refered variables
        for (auto ref: NodeQuery::querySubTree(project, V_SgVarRefExp)) {
            auto ref_v = isSgVarRefExp(ref);
            auto var = ref_v->get_symbol()->get_declaration();

            if (misc_utils::insideSystemHeader(var))
                continue; // skip system header files

            DEBUG_IF(!vars_used.count(var), "Found used variable: ") <<
                misc_utils::debug_info(var) << std::endl;
            vars_used.insert(var);
        }

        // initialized variables
        for (auto i: NodeQuery::querySubTree(project, V_SgInitializedName)) {
            auto var_v = isSgInitializedName(i);
            if (var_v->get_initializer()) vars_used.insert(var_v);
        }

        return vars_used;
    }
};
