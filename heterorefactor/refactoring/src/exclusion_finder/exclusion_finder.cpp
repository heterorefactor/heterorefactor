#include "exclusion_finder.h"

ExclusionFinder::ExclusionFinder(SgProject* project,
        misc_utils::RefactorType t) {
    m_project = project;
    m_type = t;
}

void ExclusionFinder::run(void) {
    switch (m_type) {
    case misc_utils::RefactorType::fp:
        rule_addressof();
        rule_interface();
        break;
    case misc_utils::RefactorType::rec:
        rule_addressof();
        rule_array();
        rule_interface();
        rule_propagate();
        break;
    default:
        break;
    }
}

std::set<SgNode *> ExclusionFinder::get_excluded(void) {
    return m_excluded;
}

std::vector<std::pair<SgNode *, SgExpression *> >
ExclusionFinder::get_addressof_propagated(void) {
    return m_addressof_edge;
}
