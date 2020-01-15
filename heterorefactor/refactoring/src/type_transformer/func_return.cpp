#include "type_transformer.h"

void TypeTransformer::transform_func_return(void) {
    for (auto func: ast_utils::all_used_func_def(m_project)) {
        // skip function if it is excluded
        if (m_excluded->count(func)) continue;

        // skip system
        if (misc_utils::insideSystemHeader(func)) continue;

        // function will not return an array
        // skip function if its return type is not a pointer / float

        SgType *target = NULL;
        if (isSgPointerType(func->get_type()->get_return_type())) {
            target = get_transformation_of(
                    isSgPointerType(func->get_type()->get_return_type()),
                    SageInterface::getGlobalScope(func));
        } else if (isSgType(func->get_type()->get_return_type())->isFloatType()) {
            target = get_transformation_fp(SageInterface::getGlobalScope(func));
        } else {
            continue;
        }

        INFO_IF(true, "[transform] function return type: ") <<
            misc_utils::debug_info(func) << std::endl;

        // transform function return type
        func->set_type(SageBuilder::buildFunctionType(target,
                    func->get_type()->get_argument_list()));
    }
}
