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
#define PPCAT_NX(A,B) A##B
#define PPCAT(A,B) PPCAT_NX(A,B)

#define INIT PPCAT(__dst_alloc_init, ACCESS_NAME)
#define MALLOC PPCAT(__dst_alloc_malloc, ACCESS_NAME)
#define FREE PPCAT(__dst_alloc_free, ACCESS_NAME)

#define MEM_TYPE PPCAT(__dst_alloc_list, ACCESS_NAME)

#define BUCKETS PPCAT(__dst_alloc_buckets, ACCESS_NAME)
#define NODE_SPLIT PPCAT(__dst_alloc_node_split, ACCESS_NAME)

#define TEST_PARENT_SPLIT PPCAT(__dst_alloc_test_parent_split, ACCESS_NAME)
#define FLIP_PARENT_SPLIT PPCAT(__dst_alloc_flip_parent_split, ACCESS_NAME)
#define CLR_PARENT_SPLIT PPCAT(__dst_alloc_clr_parent_split, ACCESS_NAME)
#define LIST_PUSH PPCAT(__dst_alloc_list_push, ACCESS_NAME)
#define LIST_REMOVE PPCAT(__dst_alloc_list_remove, ACCESS_NAME)
#define LIST_POP PPCAT(__dst_alloc_list_pop, ACCESS_NAME)
#define BUCKET_FOR_REQUEST PPCAT(__dst_alloc_bucket_for_request, ACCESS_NAME)
#define INDEX_FOR_NODE PPCAT(__dst_alloc_index_for_node, ACCESS_NAME)
#define NODE_FOR_INDEX PPCAT(__dst_alloc_node_for_index, ACCESS_NAME)

#define NODE_PARENT(i) (((i) - 1) / 2)
#define NODE_SLIBING(i) ((((i) - 1) ^ 1) + 1)
#define NODE_NEXT(i) ((i) + 1)
#define NODE_LEFT_CHILD(i) ((i) * 2 + 1)
// END LOCAL NAMES




#define MAX_ALLOC_ELEMENT (1 << ELEMENT_LOG2)


// __dst_alloc_size_t should has at least (ELEMENT_LOG2 + 1) bits
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
#endif



unsigned char NODE_SPLIT[(1 << (ELEMENT_LOG2)) / 8];

static unsigned char TEST_PARENT_SPLIT(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (NODE_SPLIT[index / 8] >> (index % 8)) & 1;
}

static void FLIP_PARENT_SPLIT(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    NODE_SPLIT[index / 8] ^= 1 << (index % 8);
}

static void CLR_PARENT_SPLIT(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    NODE_SPLIT[index / 8] &= ~(1 << (index % 8));
}



struct __dst_alloc_list_base_t BUCKETS[ELEMENT_LOG2 + 1] = {{1, 1}};

static __dst_alloc_bucket_size_t BUCKET_FOR_REQUEST(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = ELEMENT_LOG2;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}

static void LIST_PUSH(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {

    __dst_alloc_size_t prev = BUCKETS[bucket].prev;

    ACCESS_NAME[entry]._link.prev = prev;
    ACCESS_NAME[entry]._link.next = 0;

    if (prev == 0) BUCKETS[bucket].next = entry;
    else ACCESS_NAME[prev]._link.next = entry;
    BUCKETS[bucket].prev = entry;
}

static void LIST_REMOVE(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {

    __dst_alloc_size_t prev = ACCESS_NAME[entry]._link.prev;
    __dst_alloc_size_t next = ACCESS_NAME[entry]._link.next;

    if (prev == 0) BUCKETS[bucket].next = next;
    else ACCESS_NAME[prev]._link.next = next;

    if (next == 0) BUCKETS[bucket].prev = prev;
    else ACCESS_NAME[next]._link.prev = prev;
}

static __dst_alloc_size_t LIST_POP(
        __dst_alloc_bucket_size_t bucket) {

    __dst_alloc_size_t back = BUCKETS[bucket].prev;
    if (back == 0) return 0;
    LIST_REMOVE(bucket, back);
    return back;
}

static __dst_alloc_size_t INDEX_FOR_NODE(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (ELEMENT_LOG2 - bucket));
}

static __dst_alloc_size_t NODE_FOR_INDEX(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (ELEMENT_LOG2 - bucket)) + (1 << bucket) - 1;
}




void INIT() {
    for (int i = 0; i <= ELEMENT_LOG2; i++) {
        BUCKETS[i].prev = 0;
        BUCKETS[i].next = 0;
    }
    LIST_PUSH(0, 1);
}

__dst_alloc_size_t MALLOC(__dst_alloc_size_t request) {
    request = request / sizeof(ACCESS_TYPE);
    if (request > MAX_ALLOC_ELEMENT) return 0;

    __dst_alloc_bucket_size_t bucket =
        BUCKET_FOR_REQUEST(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;

    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = LIST_POP(bucket);
        if (!ptr) {
            bucket--; continue;
        }

        __dst_alloc_size_t i = NODE_FOR_INDEX(ptr, bucket);
        if (i != 0) FLIP_PARENT_SPLIT(i);

        while (bucket < original_bucket) {
            i = NODE_LEFT_CHILD(i);
            // initialize
            CLR_PARENT_SPLIT(i);
            FLIP_PARENT_SPLIT(i);
            bucket++;
            LIST_PUSH(bucket, INDEX_FOR_NODE(NODE_NEXT(i), bucket));
        }

        ACCESS_NAME[ptr]._link.prev = request;
        return ptr;
    }

    return 0;
}

void FREE(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;

    __dst_alloc_bucket_size_t bucket =
        BUCKET_FOR_REQUEST(ACCESS_NAME[ptr]._link.prev);

    __dst_alloc_size_t i = NODE_FOR_INDEX(ptr, bucket);

    while (i != 0) {
        FLIP_PARENT_SPLIT(i);
        if (TEST_PARENT_SPLIT(i)) break;
        LIST_REMOVE(bucket, INDEX_FOR_NODE(NODE_SLIBING(i), bucket));
        i = NODE_PARENT(i);
        bucket--;
    }

    LIST_PUSH(bucket, INDEX_FOR_NODE(i, bucket));
}


#ifdef DEBUG
#include <signal.h>
int main(void) {
    raise(SIGINT);
    return 0;
}
#endif
