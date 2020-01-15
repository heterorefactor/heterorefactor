#include "exclusion_finder.h"

void ExclusionFinder::rule_propagate() {
    auto seed = misc_utils::make_unique< std::set<SgNode *> >(m_excluded);
    auto excluding = misc_utils::make_unique< std::set<SgNode *> >();

    while (!seed->empty()) {
        std::set<SgNode *> references;
        references = ast_utils::all_reference(m_project, *seed);

        for (auto ref : references) {
            auto newpro = ast_utils::pointer_propagate(ref);
            for (auto propagated : newpro) {
                if (m_excluded.count(propagated)) continue;
                if (excluding->count(propagated)) continue;
                excluding->insert(propagated);
                INFO_IF(true, "[exclusion] rule propagate: ") <<
                    misc_utils::debug_info(propagated) << std::endl;
                INFO_IF(true, "    from: ") <<
                    misc_utils::debug_info(ref) << std::endl;

                auto from = (isSgVarRefExp(ref) ?
                        (SgNode *)isSgVarRefExp(ref)->get_symbol()
                        ->get_declaration() : (
                    isSgFunctionRefExp(ref) ?
                        (SgNode *)isSgFunctionRefExp(ref)->get_symbol()->
                        get_declaration()->get_definingDeclaration() : NULL));
                if (from && m_addressof_vis.count(from)
                        && !m_addressof_vis.count(propagated)) {
                    INFO_IF(true, "[propagation(pre)] ") <<
                        misc_utils::debug_info(propagated) << std::endl;
                    INFO_IF(true, "    from: ") <<
                        misc_utils::debug_info(ref) << std::endl;
                    m_addressof_vis.insert(propagated);
                    m_addressof_edge.push_back(std::make_pair(
                                (SgNode *)propagated, (SgExpression *)ref));
                }
            }
        }

        m_excluded.insert(excluding->begin(), excluding->end());
        seed = std::move(excluding);
        excluding = misc_utils::make_unique< std::set<SgNode *> >();
    }
}
