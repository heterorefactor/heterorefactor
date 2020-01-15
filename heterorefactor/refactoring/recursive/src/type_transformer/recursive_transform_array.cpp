#include "type_transformer.h"

SgType *TypeTransformer::recursive_transform_array(
        SgType *type, SgScopeStatement *scope) {

    if (auto arr = isSgArrayType(type)) {
        auto type = recursive_transform_array(arr->get_base_type(), scope);
        if (!type) return NULL;  // base type is not transformable

        auto newarr = SageBuilder::buildArrayType(
                type, arr->get_index());
        return newarr;

    } else if (auto ptr = isSgPointerType(type)) {
        return get_transformation_of(ptr, scope);

    } else return NULL;
}
