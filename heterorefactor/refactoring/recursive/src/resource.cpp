#include "resource.h"

namespace resource {
    const std::string runtime_alloc =
#include "generated/runtime_alloc.frag"
        ;
};
