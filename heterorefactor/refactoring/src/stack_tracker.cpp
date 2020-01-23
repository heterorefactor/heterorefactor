#include "stack_tracker.h"

StackTracker::StackTracker(SgProject* project) {
    m_project = project;
}

void StackTracker::set_target(std::set<SgFunctionDeclaration *> *target) {
    m_target = target;
}

void StackTracker::run(void) {
    for (auto func : *m_target)
        track_call_return(func);
}

void StackTracker::track_call_return(SgFunctionDeclaration *func) {
    std::string temp =
        "if (!__dst_file) { "
            "__dst_file = (unsigned long long)fopen(__dst_filename, \"w\"); }"
        "fprintf((FILE *)__dst_file, \"[__REC_RECUR] ";
    std::string temp2 =
        "\");"
        "fflush((FILE *)__dst_file);";

    // function entry
    SageInterface::attachArbitraryText(
            SageInterface::getFirstStatement(
                func->get_definition()->get_body()),
            temp + "call " +
            func->get_mangled_name().getString() + temp2);

    // transform all return
    for (auto i : NodeQuery::querySubTree(func, V_SgReturnStmt)) {
        auto ret = isSgReturnStmt(i);
        SageInterface::attachArbitraryText(ret, temp + "ret " +
                func->get_mangled_name().getString() + temp2);
    }

    // default return statement
    SageInterface::attachArbitraryText(
            SageInterface::getLastStatement(
                func->get_definition()->get_body()),
            temp + "ret " + func->get_mangled_name().getString() + temp2,
            PreprocessingInfo::after);
}
