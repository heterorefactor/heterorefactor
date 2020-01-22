#include "exclusion_finder.h"

void ExclusionFinder::rule_pointer_cast(void) {
    for (auto i: NodeQuery::querySubTree(m_project, V_SgCastExp)) {
        auto cast = isSgCastExp(i);

        for (auto j: NodeQuery::querySubTree(
                    cast->get_operand(), V_SgAddressOfOp)) {
            auto op = isSgAddressOfOp(j);

            for (auto k: NodeQuery::querySubTree(
                        op->get_operand(), V_SgVarRefExp)) {
                auto ref = isSgVarRefExp(k);

                auto sym = ref->get_symbol();
                if (sym == NULL) continue;
                auto decl = sym->get_declaration();
                if (decl == NULL) continue;

                // if &i is casted, exclude i
                // TODO: consider (int)(i + *(&a)), although uncommon
                INFO_IF(m_excluded.find(decl) == m_excluded.end(),
                        "[exclusion] rule pointer cast: ") <<
                    misc_utils::debug_info(decl) << std::endl;
                m_excluded.insert(decl);
            }
        }
    }
}
