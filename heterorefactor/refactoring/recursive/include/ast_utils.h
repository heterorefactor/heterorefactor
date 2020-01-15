#pragma once

#include "rose.h"

#include <set>

namespace ast_utils {
    std::set<SgNode *> all_reference(
            SgProject* project, std::set<SgNode *> &seed);

    std::set<SgFunctionDeclaration *>
        all_transformable_func_decl(SgProject* project);
    std::set<SgFunctionDeclaration *>
        all_used_func_def(SgProject* project);

    std::set<SgInitializedName *>
        all_used_var_decl(SgProject* project);

    std::set<SgNode *> pointer_propagate(SgNode *node);

    void transform_func_decl_matching_def(SgProject* project);
};
