#include "type_transformer.h"

TypeTransformer::TypeTransformer(SgProject* project) {
    m_project = project;
}

void TypeTransformer::set_exclusion(const std::set<SgNode *> *excluded) {
    m_excluded = excluded;
}

void TypeTransformer::transform(void) {
    transform_func_return();
    transform_func_param();
    ast_utils::transform_func_decl_matching_def(m_project);

    transform_var_decl();
    transform_typecast();
}
