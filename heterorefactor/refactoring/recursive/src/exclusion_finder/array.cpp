#include "exclusion_finder.h"

void ExclusionFinder::rule_array(void) {
    for (auto i: NodeQuery::querySubTree(m_project, V_SgStringVal)) {
        auto exp = isSgExpression(i);

        for (auto node : ast_utils::pointer_propagate(exp)) {
            INFO_IF(m_excluded.find(node) == m_excluded.end(),
                    "[exclusion] rule array: ") <<
                misc_utils::debug_info(node) << std::endl;
            m_excluded.insert(node);
        }
    }
}
