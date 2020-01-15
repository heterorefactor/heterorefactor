#include "exclusion_finder.h"

void ExclusionFinder::rule_addressof(void) {
    for (auto i: NodeQuery::querySubTree(m_project, V_SgAddressOfOp)) {
        auto op = isSgAddressOfOp(i);

        for (auto node : ast_utils::pointer_propagate(op)) {
            INFO_IF(m_excluded.find(node) == m_excluded.end(),
                    "[exclusion] rule addressof: ") <<
                misc_utils::debug_info(node) << std::endl;
            m_excluded.insert(node);

            if (!m_addressof_vis.count(node)) {
                INFO_IF(true, "[propagation(pre)] ") <<
                    misc_utils::debug_info(node) << std::endl;
                INFO_IF(true, "    from: ") <<
                    misc_utils::debug_info(op) << std::endl;
                m_addressof_vis.insert(node);
                m_addressof_edge.push_back(std::make_pair(
                            (SgNode *)node, (SgExpression *)op));
            }
        }
    }
}
