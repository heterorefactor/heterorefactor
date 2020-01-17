#pragma once

#include "rose.h"

#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#define CERR_IF(cond) ((cond) ? std::cerr : misc_utils::null)
#define INFO_IF(cond, message) \
        (CERR_IF(cond) << "[INFO] " << message)
#define WARN_IF(cond, message) \
        (CERR_IF(cond) << "[WARNING] " << message)

#ifdef DEBUG
#define DEBUG_IF(cond, message) \
        (CERR_IF(cond) << "[DEBUG] " << message)
#else
#define DEBUG_IF(cond, message) (misc_utils::null)
#endif

namespace misc_utils {
    class NullBuffer : public std::streambuf {
        public: inline int overflow(int c) { return c; }
    };

    extern NullBuffer null_buffer;
    extern std::ostream null;

    std::string debug_info(SgNode *decl);
    std::string debug_info(SgInitializedName *decl);
    std::string debug_info(SgFunctionDeclaration *decl);
    std::string debug_info(SgCastExp *cast);

    bool insideSystemHeader(SgLocatedNode* node);

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    enum RefactorType { i, fp, rec };
};
