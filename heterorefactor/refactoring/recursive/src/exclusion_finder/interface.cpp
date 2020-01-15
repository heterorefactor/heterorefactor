#include "exclusion_finder.h"

#include <boost/regex.hpp>

void ExclusionFinder::rule_interface(void) {
    for (auto i: NodeQuery::querySubTree(m_project, V_SgPragmaDeclaration)) {
        // get pragma string
        auto op = isSgPragmaDeclaration(i);
        if (op == NULL) continue;

        auto pragma_str = op->get_pragma()->get_name();

        // match for interface variable name
        static const boost::regex regex(
                "hls[[:space:]]+"
                "interface[[:space:]]+" ".*"
                "port[[:space:]]*=[[:space:]]*"
                "([a-zA-Z_][a-zA-Z0-9_]*)" ".*",
                boost::regex::icase);
        boost::match_results<std::string::const_iterator> match;
        if (!boost::regex_match(pragma_str, match, regex)) continue;
        SgName var_name(match[1]);

        // lookup variable in scopes
        auto sym = SageInterface::lookupVariableSymbolInParentScopes(
                var_name, op->get_scope());
        if (sym == NULL) continue;
        auto decl = sym->get_declaration();
        if (decl == NULL) continue;

        // check is pointer type
        if (!isSgPointerType(decl->get_type())) continue;

        // exclude pointer interface
        INFO_IF(m_excluded.find(decl) == m_excluded.end(),
                "[exclusion] rule interface: ") <<
            misc_utils::debug_info(decl) << std::endl;
        m_excluded.insert(decl);
    }
}
