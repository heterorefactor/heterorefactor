#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include <map>
#include <tuple>
#include <string>

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include "type_transformer.h"

class AccessTransformer {
public:
    struct AccessTool {
        SgInitializedName *actual_access;
        SgFunctionDeclaration *malloc_func;
        SgFunctionDeclaration *free_func;
    };

    AccessTransformer(SgProject* project);
    void set_type_transformer(TypeTransformer *type_trans);
    void set_is_instrument(void);
    void collect_access(void);
    void transform(void);

    std::tuple<SgInitializedName *, AccessTool>
        get_storage_of(SgType *type, SgScopeStatement *scope);

protected:
    bool is_access(SgExpression *exp);
    SgPointerType *get_pointer_access_type(SgExpression *exp);
    void set_pointer_access_loc(SgExpression *exp,
            SgInitializedName *mem, SgInitializedName *actual_loc);

    std::string get_alloc_runtime_of(std::string access_name,
            std::string type_str, int size_log2);

    SgProject *m_project;
    TypeTransformer *m_type_trans;
    SgClassDeclaration *m_list_base_decl;
    bool m_is_instrument;

    std::map<SgExpression *, SgPointerType *> m_original_type;
    std::map<SgType *, SgInitializedName *> m_storage;
    std::map<SgType *, AccessTool> m_access_tool;
};
