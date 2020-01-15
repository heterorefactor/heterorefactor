#include <ast_utils.h>
#include <misc_utils.h>

namespace ast_utils {
    static bool is_stop_propagation(SgNode *node,
            const std::vector<SgNode *> &visited) {
        // dereference
        if (isSgArrowExp(node)) return true;
        if (isSgArrowStarOp(node)) return true;
        if (isSgPointerDerefExp(node)) return true;
        if (isSgPntrArrRefExp(node)) return true;

        // comma
        if (auto comma = isSgCommaOpExp(node)) {
            assert(visited.size());
            return comma->get_lhs_operand() == visited.back();
        }

        // comparision
        if (isSgEqualityOp(node) || isSgNotEqualOp(node) ||
                isSgGreaterOrEqualOp(node) || isSgGreaterThanOp(node) ||
                isSgLessOrEqualOp(node) || isSgLessThanOp(node) ||
                isSgNotOp(node)) {
            WARN_IF(true, "stop propagation: ") <<
                "HLS doesn't support pointer comparison between " <<
                "native pointers (e.g. interfaces, static addresses)" <<
                std::endl;
            return true;
        }

        // sizes or types
        if (isSgSizeOfOp(node)) return true;
        if (isSgTypeIdOp(node)) return true;
        if (isSgTypeTraitBuiltinOperator(node)) return true;

        return false;
    }

    static bool is_dot_lhs(SgNode *node, SgNode *until) {
        for (auto prev = node, curr = node->get_parent();
                curr && curr != until;
                prev = prev->get_parent(),
                curr = curr->get_parent()) {
            if (!(isSgDotExp(curr) || isSgArrowExp(curr) ||
                    isSgDotStarOp(curr) || isSgArrowStarOp(curr)))
                continue;
            auto op = isSgBinaryOp(curr);
            if (prev == op->get_lhs_operand()) return true;
        }

        return false;
    }

    static void pointer_propagate_assign(
            SgNode *node, std::set<SgNode *> &result) {
        // propagate to lhs variables
        for (auto lhs_ref : NodeQuery::querySubTree(
                isSgAssignOp(node)->get_lhs_operand_i(), V_SgVarRefExp)) {
            auto lhs_ref_v = isSgVarRefExp(lhs_ref);
            // struct a b; b.c, a::c is an exception but b is not
            if (is_dot_lhs(lhs_ref_v, node)) continue;
            result.insert(lhs_ref_v->get_symbol()->get_declaration());
        }
    }

    static void pointer_propagate_init(
            SgNode *node, std::set<SgNode *> &result) {
        // propagate to initialized name
        auto in = isSgInitializedName(isSgInitializer(node)->get_parent());
        if (in) result.insert(in);
    }

    static bool pointer_propagate_call(
            SgNode *node, std::set<SgNode *> &result,
            const std::vector<SgNode *> &visited) {
        auto call = isSgCallExpression(node);

        assert(visited.size() >= 1);
        if (visited.back() == call->get_function()) return false;

        // get parameter index
        auto parlist = call->get_args();
        assert(visited.size() >= 2);
        auto affected = visited[visited.size() - 2];
        size_t index = parlist->get_childIndex(affected);

        // propagate to the corresponding parameter
        for (auto func_ref : NodeQuery::querySubTree(
                call->get_function(), V_SgFunctionRefExp)) {
            auto func_ref_v = isSgFunctionRefExp(func_ref);
            auto func = func_ref_v->get_symbol()->
                get_declaration()->get_definingDeclaration();
            auto func_v = isSgFunctionDeclaration(func);

            // undefined functions could not be transformed
            if (func_v == NULL) continue;

            result.insert(func_v->get_parameterList()->get_args()[index]);
        }
        return true;
    }

    static void pointer_propagate_return(
            SgNode *node, std::set<SgNode *> &result) {
        // propagate to the enclosing function
        while (node != NULL) {
            node = node->get_parent();
            if (!isSgFunctionDeclaration(node)) continue;
            result.insert(node); break;
        }
    }

    std::set<SgNode *> pointer_propagate(SgNode *node) {
        // set of SgInitializedName and SgFunctionDeclaration
        std::set<SgNode *> result;
        std::vector<SgNode *> visited;

        while (node != NULL) {
            visited.push_back(node);
            node = node->get_parent();

            if (is_stop_propagation(node, visited)) break;

            if (isSgCastExp(node)) result.insert(node);

            if (isSgAssignOp(node))
                pointer_propagate_assign(node, result);

            if (isSgInitializer(node))
                pointer_propagate_init(node, result);

            if (isSgCallExpression(node))
                if (pointer_propagate_call(node, result, visited))
                    break;

            if (isSgReturnStmt(node)) {
                pointer_propagate_return(node, result);
                break;
            }
        }

        return result;
    }
};
