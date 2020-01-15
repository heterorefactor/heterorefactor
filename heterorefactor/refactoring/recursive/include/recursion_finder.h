#pragma once

#include "ast_utils.h"
#include "misc_utils.h"

#include "rose.h"
#include "wholeAST.h"
#include "sageInterface.h"
#include "roseAdapter.h"

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

class RecursionFinder {
public:
    RecursionFinder(SgProject* project);
    void run(void);
    std::set<SgFunctionDeclaration *>
        get_recursion_functions(void);

protected:
    SgProject* m_project;
    std::set<SgFunctionDeclaration *> m_recursion;
};
