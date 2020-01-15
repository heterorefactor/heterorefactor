#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <vector>

class LocalVarPacker {
public:
    LocalVarPacker(SgProject* project);
    void set_target(std::set<SgFunctionDeclaration *> *target);
    void run(void);

protected:
    typedef std::map<SgInitializedName *, SgInitializedName *>
        localvar_mapping_t;
    typedef std::vector<SgLabelStatement *> location_mapping_t;

    struct function_info_t {
        SgClassDeclaration *packed_type;
        localvar_mapping_t localvar_mapping;
        SgInitializedName *location_field;
        SgInitializedName *return_field;

        SgInitializedName *packed_name;
        SgInitializedName *stack_top_name;

        SgStatement *last_entry_line;
        location_mapping_t location_mapping;
    };

    SgProject *m_project;
    std::set<SgFunctionDeclaration *> *m_target;
    std::map<SgFunctionDeclaration *, function_info_t>
        m_func_info;
    int m_uid_counter;

    void pack_function(SgFunctionDeclaration *func);
    void generate_packed_type(SgFunctionDeclaration *func);
    void declare_packed_name(SgFunctionDeclaration *func);

    void transform_reference(SgFunctionDeclaration *func);
    void transform_initialization(SgFunctionDeclaration *func);
    void transform_parameter(SgFunctionDeclaration *func);
    void transform_call_return(SgFunctionDeclaration *func);

    SgExpression *get_current_stack_top(SgFunctionDeclaration *func);
    SgExpression *get_stack_pack(SgFunctionDeclaration *func, int offset);
    SgExpression *get_access(SgFunctionDeclaration *func,
            SgInitializedName *name, int offset);
    SgExpression *get_field_access(SgFunctionDeclaration *func,
            SgInitializedName *name, int offset);
    SgExpression *get_current_location(SgFunctionDeclaration *func);
    SgExpression *get_return_val(SgFunctionDeclaration *func, int offset);

    SgStatement *get_push_stack_statement(SgFunctionDeclaration *func);
    SgStatement *get_pop_stack_statement(SgFunctionDeclaration *func);
    SgStatement *get_set_location_statement(SgFunctionDeclaration *func,
            unsigned int val);

    SgLabelStatement *create_label(SgFunctionDeclaration *func);
    SgExpression *create_function_call(SgFunctionDeclaration *func,
            SgFunctionCallExp *call);
    void create_function_return(SgFunctionDeclaration *func,
            SgReturnStmt *ret);

    void add_stmt_to_entry(SgFunctionDeclaration *func, SgStatement *s);
};
