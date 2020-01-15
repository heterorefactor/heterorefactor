#include "misc_utils.h"

namespace misc_utils {
    NullBuffer null_buffer;
    std::ostream null(&null_buffer);

    std::string debug_info(SgNode *node) {
        if (auto decl = isSgInitializedName(node)) {
            return misc_utils::debug_info(decl);
        } else if (auto func = isSgFunctionDeclaration(node)) {
            return misc_utils::debug_info(func);
        } else if (auto var_ref = isSgVarRefExp(node)) {
            return misc_utils::debug_info(
                    var_ref->get_symbol()->get_declaration());
        } else if (auto func_ref = isSgFunctionRefExp(node)) {
            return misc_utils::debug_info(func_ref->get_symbol()->
                    get_declaration()->get_definingDeclaration());
        } else if (auto cast = isSgCastExp(node)) {
            return misc_utils::debug_info(cast);
        } else {
            std::stringstream ss;
            ss << "unknown node in " <<
                node->get_file_info()->get_filenameString()
                << ":" << node->get_file_info()->get_line();
            return ss.str();
        }
    }

    std::string debug_info(SgInitializedName *decl) {
        std::stringstream ss;
        ss << "variable " << decl->get_qualified_name() << " in " <<
            decl->get_declaration()->get_file_info()->get_filenameString()
            << ":" << decl->get_declaration()->get_file_info()->get_line();
        return ss.str();
    }

    std::string debug_info(SgFunctionDeclaration *decl) {
        std::stringstream ss;
        ss << "function " << decl->get_qualified_name() << " in " <<
            decl->get_file_info()->get_filenameString()
            << ":" << decl->get_file_info()->get_line();
        return ss.str();
    }

    std::string debug_info(SgCastExp *cast) {
        std::stringstream ss;
        ss << "cast to type " << cast->get_type()->unparseToString()
            << " in " << cast->get_file_info()->get_filenameString()
            << ":" << cast->get_file_info()->get_line();
        return ss.str();
    }

    bool insideSystemHeader(SgLocatedNode* node) {
        bool rtval = false;
        ROSE_ASSERT (node != NULL);
        Sg_File_Info* finfo = node->get_file_info();
        if (finfo!=NULL)
        {
            std::string fname = finfo->get_filenameString();
            std::string buildtree_str1("include-staging/gcc_HEADERS");
            std::string buildtree_str2("include-staging/g++_HEADERS");
            std::string installtree_str1("include/edg/gcc_HEADERS");
            std::string installtree_str2("include/edg/g++_HEADERS");
            std::string systemtree("/usr/include");
            std::string opttree("/opt/");
            // if the file name has a sys header path
            if ((fname.find (buildtree_str1, 0) != std::string::npos) ||
                (fname.find (buildtree_str2, 0) != std::string::npos) ||
                (fname.find (installtree_str1, 0) != std::string::npos) ||
                (fname.find (installtree_str2, 0) != std::string::npos) ||
                (fname.find (systemtree, 0) != std::string::npos) ||
                (fname.find (opttree, 0) != std::string::npos)
                )
                rtval = true;
            if (finfo->isCompilerGenerated())
                rtval = true;
        }
        return rtval;
    }
};
