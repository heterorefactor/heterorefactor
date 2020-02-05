// BEGIN PARAMETERS
// The maximum allocation size is currently assumed to be 1 M elements
// ELEMENT_LOG2 should be <= 20
#ifndef ELEMENT_LOG2
#define ELEMENT_LOG2 (20)
#endif
#ifndef ACCESS_NAME
#define ACCESS_NAME __dmemUi
#endif
#ifndef ACCESS_TYPE
#define ACCESS_TYPE unsigned int
#endif
// END PARAMETERS

// BEGIN LOCAL NAMES
#define XSTR(s) STR(s)
#define STR(s) #s
#define PPCAT_NX(A,B) A##B
#define PPCAT(A,B) PPCAT_NX(A,B)
#define MALLOC PPCAT(__dst_alloc_malloc, ACCESS_NAME)
#define FREE PPCAT(__dst_alloc_free, ACCESS_NAME)
#define MEM_TYPE PPCAT(__dst_alloc_list, ACCESS_NAME)
// END LOCAL NAMES

#define MAX_ALLOC_ELEMENT (1 << ELEMENT_LOG2)
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;

#ifdef DEBUG
struct __dst_alloc_list_base_t {
    __dst_alloc_size_t prev;
    __dst_alloc_size_t next;
};
struct MEM_TYPE {
    struct __dst_alloc_list_base_t _link;
    ACCESS_TYPE _data;
};
struct MEM_TYPE ACCESS_NAME[MAX_ALLOC_ELEMENT + 1];

#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "/tmp/hetero-profile.trace";
unsigned long long __dst_file = 0;
#endif

__dst_alloc_size_t MALLOC(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(ACCESS_TYPE);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " XSTR(ACCESS_NAME)
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    MEM_TYPE *allocated = (MEM_TYPE *)malloc(
            request * sizeof(MEM_TYPE));
    allocated[0]._link.prev = request;
    return allocated - ACCESS_NAME;
}

void FREE(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;

    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = ACCESS_NAME[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " XSTR(ACCESS_NAME)
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(ACCESS_NAME + ptr);
}


#ifdef DEBUG
#include <signal.h>
int main(void) {
    raise(SIGINT);
    return 0;
}
#endif
