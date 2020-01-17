#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include <set>
#include <vector>
#include <utility>

class ExclusionFinder {
public:
    ExclusionFinder(SgProject* project, misc_utils::RefactorType t);
    void run(void);
    std::set<SgNode *> get_excluded(void);
    std::vector<std::pair<SgNode *, SgExpression *> >
        get_addressof_propagated(void);

protected:
    void rule_interface(void);
    void rule_addressof(void);
    void rule_array(void);
    void rule_propagate(void);

    SgProject* m_project;
    misc_utils::RefactorType m_type;
    std::set<SgNode *> m_excluded;
    std::set<SgNode *> m_addressof_vis;
    std::vector<std::pair<SgNode *, SgExpression *> >
        m_addressof_edge;
};
