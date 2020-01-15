#include "ast_utils.h"
#include "misc_utils.h"

namespace ast_utils {
    std::set<SgFunctionDeclaration *>
        all_transformable_func_decl(SgProject* project) {

        std::set<SgFunctionDeclaration *> results;
        auto function_used = all_used_func_def(project);

        for (auto i: NodeQuery::querySubTree(project, V_SgFunctionDeclaration)) {
            auto func = isSgFunctionDeclaration(i);
            auto def = isSgFunctionDeclaration(func->get_definingDeclaration());

            // skip functions without definition
            if (!def) {
                DEBUG_IF(true, "function undefined: ") <<
                    misc_utils::debug_info(func) << std::endl;
                continue;
            }

            if (misc_utils::insideSystemHeader(func))
                continue; // skip system header files

            // skip function if it is not used
            if (!function_used.count(def)) continue;
            DEBUG_IF(true, "function used ") << function_used.count(def) <<
                " times: " << misc_utils::debug_info(func) << std::endl;

            // add to result
            results.insert(func);
        }

        return results;
    }

    std::set<SgFunctionDeclaration *> all_used_func_def(SgProject* project) {

        std::set<SgFunctionDeclaration *> function_used;
        for (auto ref: NodeQuery::querySubTree(project, V_SgFunctionRefExp)) {
            auto ref_v = isSgFunctionRefExp(ref);
            auto func = ref_v->get_symbol()->
                get_declaration()->get_definingDeclaration();
            auto func_v = isSgFunctionDeclaration(func);
            if (!func_v) continue;  // skip functions without definition

            if (func_v->get_file_info()->isCompilerGenerated())
                continue; // skip system header files

            DEBUG_IF(!function_used.count(func_v), "Found used function: ") <<
                misc_utils::debug_info(func_v) << std::endl;
            function_used.insert(func_v);
        }
        return function_used;
    }
};
