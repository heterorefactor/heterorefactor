#include "type_transformer.h"

void TypeTransformer::transform_func_return(void) {
    for (auto func: ast_utils::all_used_func_def(m_project)) {
        // skip function if it is excluded
        if (m_excluded->count(func)) continue;

        // skip system
        if (misc_utils::insideSystemHeader(func)) continue;

        // function will not return an array
        // skip function if its return type is not a pointer
        if (!isSgPointerType(func->get_type()->get_return_type())) continue;

        INFO_IF(true, "[transform] function return type: ") <<
            misc_utils::debug_info(func) << std::endl;

        // transform function return type
        func->set_type(SageBuilder::buildFunctionType(
                    get_transformation_of(
                        isSgPointerType(func->get_type()->get_return_type()),
                        SageInterface::getGlobalScope(func)),
                    func->get_type()->get_argument_list()));
    }
}
