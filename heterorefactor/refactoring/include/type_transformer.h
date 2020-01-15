#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include <set>
#include <map>
#include <vector>
#include <utility>

class TypeTransformer {
public:
    TypeTransformer(SgProject* project);
    void set_exclusion(const std::set<SgNode *> *excluded);
    void transform(void);

    SgType *get_transformation_fp(SgScopeStatement *scope);
    SgType *get_transformation_of(SgPointerType *type,
            SgScopeStatement *scope = NULL);

protected:
    void transform_func_return(void);
    void transform_func_param(void);
    void transform_var_decl(void);
    void transform_typecast(void);

    void transform_binary_operations(void);

    SgType *recursive_transform_array(SgType *type, SgScopeStatement *scope);

    SgProject *m_project;
    SgType *m_transform_type;
    const std::set<SgNode *> *m_excluded;

    std::map<SgPointerType *, SgType *> m_mapping_to_index;
};
