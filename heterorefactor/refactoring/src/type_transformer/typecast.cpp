#include "type_transformer.h"

void TypeTransformer::transform_typecast(void) {
    for (auto i: NodeQuery::querySubTree(m_project, V_SgCastExp)) {
        // skip cast if excluded
        if (m_excluded->count(i)) continue;

        // skip system
        auto cast = isSgCastExp(i);
        if (misc_utils::insideSystemHeader(cast)) continue;

        // could not cast to an array.
        // skip cast if not a pointer / float
        SgType *target = NULL;
        if (m_type == misc_utils::RefactorType::rec &&
                isSgPointerType(cast->get_type())) {
            target = get_transformation_of(
                    isSgPointerType(cast->get_type()),
                    SageInterface::getGlobalScope(cast));

        } else if (m_type == misc_utils::RefactorType::fp &&
                isSgType(cast->get_type())->isFloatType()) {
            target = get_transformation_fp(
                    SageInterface::getGlobalScope(cast));

        } else {
            continue;
        }

        INFO_IF(true, "[transform] cast type: ") <<
            misc_utils::debug_info(cast) << std::endl;

        // transform cast type
        cast->set_type(target);
    }
}
