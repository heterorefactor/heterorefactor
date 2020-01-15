#include "exclusion_finder.h"

ExclusionFinder::ExclusionFinder(SgProject* project) {
    m_project = project;
}

void ExclusionFinder::run(void) {
    rule_addressof();
    rule_array();
    rule_interface();
    rule_propagate();
}

std::set<SgNode *> ExclusionFinder::get_excluded(void) {
    return m_excluded;
}

std::vector<std::pair<SgNode *, SgExpression *> >
ExclusionFinder::get_addressof_propagated(void) {
    return m_addressof_edge;
}
