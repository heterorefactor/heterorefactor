#include "localvar_packer.h"

LocalVarPacker::LocalVarPacker(SgProject* project) {
    m_project = project;
    m_uid_counter = 0;
}

void LocalVarPacker::set_target(std::set<SgFunctionDeclaration *> *target) {
    m_target = target;
}

void LocalVarPacker::set_func_depth(std::map<std::string, int> mapping) {
    m_func_depth_mapping = mapping;
}

void LocalVarPacker::run(void) {
    for (auto func : *m_target)
        pack_function(func);
}

void LocalVarPacker::pack_function(SgFunctionDeclaration *func) {
    generate_packed_type(func);
    declare_packed_name(func);
    transform_reference(func);
    transform_initialization(func);
    transform_parameter(func);
    transform_call_return(func);
}

void LocalVarPacker::generate_packed_type(SgFunctionDeclaration *func) {
    auto scope = SageInterface::getGlobalScope(func);

    // Build packed type name
    std::string packed_name = "__rect_packed_type_" +
        func->get_mangled_name().getString();

    // Build packed structure
    auto packed_decl = SageBuilder::buildClassDeclaration_nfi(
            packed_name, SgClassDeclaration::e_struct, scope, NULL, NULL, NULL);
    packed_decl->unsetForward();
    // Build definition
    auto packed_def = SageBuilder::buildClassDefinition(packed_decl);

    // Store structure type for function
    m_func_info[func].packed_type = packed_decl;

    int name_counter = 0;
    for (auto i : NodeQuery::querySubTree(func, V_SgInitializedName)) {
        auto init = isSgInitializedName(i);
        auto field_name = "local" + std::to_string(name_counter++);

        INFO_IF(true, "[pack] packing: ") <<
            misc_utils::debug_info(init) << std::endl;
        auto field_decl = SageBuilder::buildVariableDeclaration(
                field_name, init->get_type(), NULL, packed_def);
        packed_def->append_member(field_decl);

        // Store mapping to the field
        auto field = field_decl->get_decl_item(field_name);
        INFO_IF(true, "    packed into: ") <<
            misc_utils::debug_info(field) << std::endl;
        m_func_info[func].localvar_mapping[init] = field;
    }

    // Build location storage
    auto field_decl = SageBuilder::buildVariableDeclaration(
            "_location", SgTypeUnsignedInt::createType(), NULL, packed_def);
    packed_def->append_member(field_decl);
    auto field = field_decl->get_decl_item("_location");
    INFO_IF(true, "[pack] creating location var: ") << field << std::endl;
    m_func_info[func].location_field = field;

    // Build return value storage
    auto return_type = func->get_type()->get_return_type();
    if (!isSgTypeVoid(return_type)) {
        field_decl = SageBuilder::buildVariableDeclaration(
                "_return", return_type, NULL, packed_def);
        packed_def->append_member(field_decl);
        field = field_decl->get_decl_item("_return");
        INFO_IF(true, "[pack] creating return var: ") << field << std::endl;
        m_func_info[func].return_field = field;
    } else {
        m_func_info[func].return_field = NULL;
    }
}

void LocalVarPacker::declare_packed_name(SgFunctionDeclaration *func) {
    auto func_scope = func->get_definition()->get_body();

    // Insert type
    auto packed_type = m_func_info[func].packed_type;
    SageInterface::insertStatementBefore(func, packed_type);

    std::string mangled_name = func->get_mangled_name().getString();
    int recur_size = 10;  // default value if no invariant available
    if (m_func_depth_mapping.find(mangled_name) !=
            m_func_depth_mapping.end()) {
        recur_size = m_func_depth_mapping[mangled_name];
    }

    // Build and insert packed stack array
    auto packed_array = SageBuilder::buildArrayType(
            SageBuilder::buildVolatileType(packed_type->get_type()),
        SageBuilder::buildUnsignedIntVal(1 << recur_size));
    auto packed_name = "__rect_packed_var_" + mangled_name;
    auto packed_decl = SageBuilder::buildVariableDeclaration(
            packed_name, packed_array, NULL, func_scope);
    auto packed_var = packed_decl->get_decl_item(packed_name);
    m_func_info[func].packed_name = packed_var;

    SageInterface::insertStatementBefore(
            SageInterface::getFirstStatement(func_scope), packed_decl);
    m_func_info[func].last_entry_line = packed_decl;

    // Build and insert stack top
    auto stack_top_name = "__rect_packed_top_" + mangled_name;
    auto stack_top_decl = SageBuilder::buildVariableDeclaration(
            stack_top_name,
            SageBuilder::buildVolatileType(SgTypeUnsignedInt::createType()),
            SageBuilder::buildAssignInitializer(
                SageBuilder::buildUnsignedIntVal(0)), func_scope);
    auto stack_top_var = stack_top_decl->get_decl_item(stack_top_name);
    m_func_info[func].stack_top_name = stack_top_var;

    add_stmt_to_entry(func, stack_top_decl);

    INFO_IF(true, "[pack] declaring function packed name: ") <<
        misc_utils::debug_info(packed_var) << std::endl;
}

void LocalVarPacker::transform_reference(SgFunctionDeclaration *func) {
    for (auto i : NodeQuery::querySubTree(func, V_SgVarRefExp)) {
        auto ref = isSgVarRefExp(i);
        auto name = ref->get_symbol()->get_declaration();

        // skip if name not mapped
        if (!m_func_info[func].localvar_mapping.count(name)) continue;

        INFO_IF(true, "[pack] replacing variable reference: ") <<
            misc_utils::debug_info(ref) << std::endl;

        auto actual_access = get_access(func, name, 0);
        SageInterface::replaceExpression(ref, actual_access);
    }
}

void LocalVarPacker::transform_initialization(SgFunctionDeclaration *func) {
    for (auto i : NodeQuery::querySubTree(func, V_SgVariableDeclaration)) {
        auto decl = isSgVariableDeclaration(i);
        bool not_mapped = false;

        // transform all declared variables
        for (auto j : decl->get_variables()) {
            auto name = isSgInitializedName(j);

            // skip if name not mapped
            if (!m_func_info[func].localvar_mapping.count(name)) {
                not_mapped = true; continue;
            }

            INFO_IF(true, "[pack] replacing initialization: ") <<
                misc_utils::debug_info(name) << std::endl;
            auto actual_access = get_access(func, name, 0);

            // create alternatives for initialize
            auto init = name->get_initializer();
            if (auto assign_orig = isSgAssignInitializer(init)) {
                auto assign_op = SageBuilder::buildAssignOp(
                        actual_access, assign_orig->get_operand());
                auto statement = SageBuilder::buildExprStatement(assign_op);
                SageInterface::insertStatementBefore(decl, statement);
            } else {
                WARN_IF(true, "[pack] failed to transform"
                        "(not an assignment):  ") <<
                    misc_utils::debug_info(name) << std::endl;
                not_mapped = true;
            }
        }

        // remove the declaration
        if (!not_mapped)
            SageInterface::removeStatement(decl);
    }
}

void LocalVarPacker::transform_parameter(SgFunctionDeclaration *func) {
    // initialize location
    auto loc_init = get_set_location_statement(func, 1);
    add_stmt_to_entry(func, loc_init);

    for (auto i : func->get_args()) {
        auto name = isSgInitializedName(i);
        INFO_IF(true, "[pack] transforming parameter: ") <<
            misc_utils::debug_info(name) << std::endl;

        auto actual_access = get_access(func, name, 0);
        auto assign_op = SageBuilder::buildAssignOp(
                actual_access, SageBuilder::buildVarRefExp(name));
        auto statement = SageBuilder::buildExprStatement(assign_op);
        add_stmt_to_entry(func, statement);
    }

    auto push_stack = get_push_stack_statement(func);
    add_stmt_to_entry(func, push_stack);
}

void LocalVarPacker::transform_call_return(SgFunctionDeclaration *func) {
    // label the arbiter
    auto label = create_label(func);
    add_stmt_to_entry(func, label);
    // reserve the start point
    auto label_start_point = create_label(func);

    // transform all call
    for (auto i : NodeQuery::querySubTree(func, V_SgFunctionCallExp)) {
        auto call = isSgFunctionCallExp(i);
        bool is_recursion_call = false;

        for (auto j : NodeQuery::querySubTree(
                    call->get_function(), V_SgFunctionRefExp)) {
            auto ref = isSgFunctionRefExp(j);
            if (ref->get_symbol()->get_declaration()
                    ->get_definingDeclaration() != func) continue;

            INFO_IF(true, "[pack] transforming function call: ") <<
                misc_utils::debug_info(ref) << std::endl;
            is_recursion_call = true;
            break;
        }

        if (is_recursion_call) {
            auto expr = create_function_call(func, call);
            SageInterface::replaceExpression(call, expr);
        }
    }

    // transform all return
    for (auto i : NodeQuery::querySubTree(func, V_SgReturnStmt)) {
        auto ret = isSgReturnStmt(i);
        create_function_return(func, ret);
        SageInterface::removeStatement(ret);
    }

    // return if no element in stack
    auto if_stmt = SageBuilder::buildIfStmt(
            SageBuilder::buildExprStatement(SageBuilder::buildEqualityOp(
                    SageBuilder::buildUnsignedIntVal(0),
                    get_current_stack_top(func))),
            SageBuilder::buildReturnStmt(get_return_val(func, 0)),
            NULL);
    add_stmt_to_entry(func, if_stmt);

    // stack_top--;
    add_stmt_to_entry(func, get_pop_stack_statement(func));

    // goto labels based on locations
    for (auto i = 1U; i < m_func_info[func].location_mapping.size(); i++) {
        auto label = m_func_info[func].location_mapping[i];
        auto if_stmt = SageBuilder::buildIfStmt(
            SageBuilder::buildExprStatement(SageBuilder::buildEqualityOp(
                SageBuilder::buildUnsignedIntVal(i),
                get_current_location(func))),
            SageBuilder::buildGotoStatement(label), NULL);
        add_stmt_to_entry(func, if_stmt);
    }

    // L1:
    add_stmt_to_entry(func, label_start_point);

    // default return statement
    SageInterface::insertStatementAfter(
        SageInterface::getLastStatement(
            func->get_definition()->get_body()),
        SageBuilder::buildGotoStatement(
            m_func_info[func].location_mapping[0]));
}

SgExpression *LocalVarPacker::get_current_stack_top(
        SgFunctionDeclaration *func) {
    auto scope = func->get_definition()->get_body();
    return SageBuilder::buildVarRefExp(
            m_func_info[func].stack_top_name, scope);
}

SgExpression *LocalVarPacker::get_stack_pack(
        SgFunctionDeclaration *func, int offset) {
    auto scope = func->get_definition()->get_body();
    return SageBuilder::buildPntrArrRefExp(
            SageBuilder::buildVarRefExp(
                m_func_info[func].packed_name, scope),
            SageBuilder::buildAddOp(
                SageBuilder::buildIntVal(offset),
                get_current_stack_top(func)));
}

SgExpression *LocalVarPacker::get_field_access(SgFunctionDeclaration *func,
        SgInitializedName *name, int offset) {
    auto scope = func->get_definition()->get_body();
    return SageBuilder::buildDotExp(get_stack_pack(func, offset),
            SageBuilder::buildVarRefExp(name, scope));
}

SgExpression *LocalVarPacker::get_access(SgFunctionDeclaration *func,
        SgInitializedName *name, int offset) {
    return get_field_access(func,
            m_func_info[func].localvar_mapping[name], offset);
}

SgExpression *LocalVarPacker::get_current_location(
        SgFunctionDeclaration *func) {
    auto scope = func->get_definition()->get_body();
    return SageBuilder::buildDotExp(get_stack_pack(func, 0),
            SageBuilder::buildVarRefExp(
                m_func_info[func].location_field, scope));
}

SgExpression *LocalVarPacker::get_return_val(
        SgFunctionDeclaration *func, int offset) {
    auto scope = func->get_definition()->get_body();
    if (!m_func_info[func].return_field) return NULL;
    else return SageBuilder::buildDotExp(get_stack_pack(func, offset),
            SageBuilder::buildVarRefExp(
                m_func_info[func].return_field, scope));
}

SgStatement *LocalVarPacker::get_push_stack_statement(
        SgFunctionDeclaration *func) {
    return SageBuilder::buildExprStatement(
            SageBuilder::buildPlusPlusOp(
                get_current_stack_top(func)));
}

SgStatement *LocalVarPacker::get_pop_stack_statement(
        SgFunctionDeclaration *func) {
    return SageBuilder::buildExprStatement(
            SageBuilder::buildMinusMinusOp(
                get_current_stack_top(func)));
}

SgStatement *LocalVarPacker::get_set_location_statement(
        SgFunctionDeclaration *func, unsigned int val) {
    return SageBuilder::buildExprStatement(
            SageBuilder::buildAssignOp(get_current_location(func),
                SageBuilder::buildUnsignedIntVal(val)));
}

SgLabelStatement *LocalVarPacker::create_label(
        SgFunctionDeclaration *func) {
    auto size = std::to_string(m_func_info[func].location_mapping.size());
    auto label_name = "__rect_func_L" + size + "_" +
        func->get_mangled_name().getString();
    auto label = SageBuilder::buildLabelStatement(
            label_name, NULL, func->get_definition());
    m_func_info[func].location_mapping.push_back(label);
    return label;
}

SgExpression *LocalVarPacker::create_function_call(
        SgFunctionDeclaration *func, SgFunctionCallExp *call) {
    auto func_scope = func->get_definition()->get_body();

    auto curr = call->get_parent();
    while (curr && !isSgStatement(curr)) curr = curr->get_parent();
    auto stmt = isSgStatement(curr);

    auto label = create_label(func);

    // TODO: if (stack_top + 1 == 1 << 10) g_fallback = true;
    /*
    auto empty_stmt = SageBuilder::buildExprStatement(
        SageBuilder::buildIntVal(0));
    SageInterface::insertStatementBefore(stmt,
            SageBuilder::buildIfStmt(
                SageBuilder::buildEqualityOp(
                    SageBuilder::buildAddOp(
                        get_current_stack_top(func),
                        SageBuilder::buildUnsignedIntVal(1)),
                    SageBuilder::buildUnsignedIntVal(1 << 10)),
                empty_stmt, NULL));
    SageInterface::attachArbitraryText(empty_stmt, "g_fallback = true;");
    */

    // packed[stack_top].location = ?;
    SageInterface::insertStatementBefore(stmt,
            SageBuilder::buildExprStatement(
                SageBuilder::buildAssignOp(
                    get_current_location(func),
                    SageBuilder::buildUnsignedIntVal(
                        m_func_info[func].location_mapping.size() - 1))));

    // pass parameters
    auto args_expr = call->get_args()->get_expressions();
    auto args_var = func->get_args();
    for (int i = args_expr.size() - 1; i >= 0; i--) {
        SageInterface::insertStatementBefore(stmt,
                SageBuilder::buildExprStatement(
                    SageBuilder::buildAssignOp(
                        get_access(func, args_var[i], 1),
                        SageInterface::deepCopy(args_expr[i]))));
    }

    // stack_top++;
    SageInterface::insertStatementBefore(stmt,
            get_push_stack_statement(func));

    // packed[stack_top].location = 1;
    SageInterface::insertStatementBefore(stmt,
            SageBuilder::buildExprStatement(
                SageBuilder::buildAssignOp(
                    get_current_location(func),
                    SageBuilder::buildUnsignedIntVal(1))));

    // goto L1; just start it without going to arbiter
    SageInterface::insertStatementBefore(stmt,
            SageBuilder::buildGotoStatement(
                m_func_info[func].location_mapping[1]));

    // label the location:
    SageInterface::insertStatementBefore(stmt, label);

    auto return_val = get_return_val(func, 1);
    if (return_val) {
        // int return_val_uid0 = packed[stack_top + 1].return_val;
        auto return_name = "_ret" + std::to_string(m_uid_counter++);
        auto return_type = func->get_type()->get_return_type();
        auto return_decl = SageBuilder::buildVariableDeclaration(
                return_name, return_type, NULL, func_scope);
        m_func_info[func].packed_type->get_definition()->
            append_member(return_decl);
        auto return_var = return_decl->get_decl_item(return_name);

        auto return_value = SageBuilder::buildAssignInitializer(
                return_val, return_type);
        auto return_get = SageBuilder::buildExprStatement(
                SageBuilder::buildAssignOp(
                    get_field_access(func, return_var, 0),
                    return_value));
        SageInterface::insertStatementBefore(stmt, return_get);
        return get_field_access(func, return_var, 0);
    } else {
        return SageBuilder::buildIntVal(0);
    }
}

void LocalVarPacker::create_function_return(
        SgFunctionDeclaration *func, SgReturnStmt *ret) {
    // set return value
    auto return_val = get_return_val(func, 0);
    if (return_val) {
        SageInterface::insertStatementBefore(ret,
            SageBuilder::buildExprStatement(
                SageBuilder::buildAssignOp(
                    return_val, ret->get_expression())));
    }

    // goto L0
    SageInterface::insertStatementBefore(ret,
        SageBuilder::buildGotoStatement(
            m_func_info[func].location_mapping[0]));

}

void LocalVarPacker::add_stmt_to_entry(
        SgFunctionDeclaration *func, SgStatement *s) {
    SageInterface::insertStatementAfter(
            m_func_info[func].last_entry_line, s);
    m_func_info[func].last_entry_line = s;
}
