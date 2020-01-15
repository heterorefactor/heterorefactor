#include "ast_utils.h"
#include "misc_utils.h"

namespace ast_utils {
    void transform_func_decl_matching_def(SgProject* project) {
        for (auto func: ast_utils::all_transformable_func_decl(project)) {
            // skip definition
            if (!func->isForward()) continue;

            // skip if no definition
            auto def = isSgFunctionDeclaration(func->get_definingDeclaration());
            if (!def) continue;

            INFO_IF(true, "[func_decl] ") <<
                misc_utils::debug_info(func) << std::endl;

            // replace function parameter types
            auto args = def->get_parameterList()->get_args();
            for (size_t i = 0; i < args.size(); i++) {
                auto type = args[i]->get_type();
                func->get_parameterList()->get_args()[i]->set_type(type);
                func->get_parameterList_syntax()->get_args()[i]->set_type(type);
            }

            // replace function types
            auto func_partype = SageBuilder::buildFunctionParameterTypeList(
                    func->get_parameterList());
            auto func_type = SageBuilder::buildFunctionType(
                    def->get_type()->get_return_type(), func_partype);
            auto func_type_syntax = SageBuilder::buildFunctionType(
                    def->get_type()->get_return_type(), func_partype);
            func->set_type(func_type);
            func->set_type_syntax(func_type_syntax);
        }
    }
};
