#include "type_transformer.h"

void TypeTransformer::transform_typecast(void) {
    for (auto i: NodeQuery::querySubTree(m_project, V_SgCastExp)) {
        // skip cast if excluded
        if (m_excluded->count(i)) continue;

        // could not cast to an array.
        // skip cast if not a pointer
        auto cast = isSgCastExp(i);
        if (!isSgPointerType(cast->get_type())) continue;

        // skip system
        if (misc_utils::insideSystemHeader(cast)) continue;

        INFO_IF(true, "[transform] cast type: ") <<
            misc_utils::debug_info(cast) << std::endl;

        // transform cast type
        cast->set_type(get_transformation_of(
                    isSgPointerType(cast->get_type()),
                    SageInterface::getGlobalScope(cast)));
    }
}
