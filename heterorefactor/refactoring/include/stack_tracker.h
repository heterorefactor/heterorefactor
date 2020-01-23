#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <vector>

class StackTracker {
public:
    StackTracker(SgProject* project);
    void set_target(std::set<SgFunctionDeclaration *> *target);
    void run(void);

protected:
    SgProject *m_project;
    std::set<SgFunctionDeclaration *> *m_target;

    void track_call_return(SgFunctionDeclaration *func);
};
