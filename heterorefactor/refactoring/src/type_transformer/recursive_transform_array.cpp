#include "type_transformer.h"

SgType *TypeTransformer::recursive_transform_array(
        SgType *type, SgScopeStatement *scope) {

    if (auto arr = isSgArrayType(type)) {
        auto type = recursive_transform_array(arr->get_base_type(), scope);
        if (!type) return NULL;  // base type is not transformable

        auto newarr = SageBuilder::buildArrayType(
                type, arr->get_index());
        return newarr;

    } else if (isSgPointerType(type) &&
            m_type == misc_utils::RefactorType::rec) {
        auto ptr = isSgPointerType(type);
        return get_transformation_of(ptr, scope);

    } else if (isSgType(type)->isFloatType() &&
            m_type == misc_utils::RefactorType::fp) {
        return get_transformation_fp(scope);

    } else return NULL;
}
