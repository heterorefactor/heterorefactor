#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include <set>
#include <map>
#include <vector>
#include <utility>

class TransformPropagator {
public:
    TransformPropagator(SgProject* project);
    void set_addressof_propagated(
            const std::vector<std::pair<SgNode *, SgExpression *> > *edges);
    void propagate(void);

protected:
    SgProject *m_project;
    const std::vector<std::pair<SgNode *, SgExpression *> >
        *m_addressof_edge;
};
