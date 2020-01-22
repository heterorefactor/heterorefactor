#include "resource.h"

namespace resource {
    const std::string runtime_alloc =
#include "generated/runtime_alloc.frag"
        ;
    const std::string trace_alloc =
#include "generated/trace_alloc.frag"
        ;
};
