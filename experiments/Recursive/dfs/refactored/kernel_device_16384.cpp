
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxi was int *
typedef unsigned long long __didxi;
// type __didxL105R was ::node*
typedef unsigned long long __didxL105R;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;
// === END REC SUPPORT LIBRARY ===
#include <stdlib.h>
#include <stdio.h>

struct __dst_alloc_list__dmemi 
{
  struct __dst_alloc_list_base_t _link;
  int _data;
}
;
struct __dst_alloc_list__dmemi __dmemi[32768U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 54 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 73 "<stdin>"
unsigned char __dst_alloc_node_split__dmemi[(1 << (15 - 1)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemi(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemi[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemi(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemi[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemi(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemi[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemi[15] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemi(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 15 - 1;
    __dst_alloc_size_t size = 2;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemi(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemi[bucket].prev;
    __dmemi[entry]._link.prev = prev;
    __dmemi[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemi[bucket].next = entry;
    else __dmemi[prev]._link.next = entry;
    __dst_alloc_buckets__dmemi[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemi(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemi[entry]._link.prev;
    __dst_alloc_size_t next = __dmemi[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemi[bucket].next = next;
    else __dmemi[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemi[bucket].prev = prev;
    else __dmemi[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemi(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemi[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemi(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemi(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (15 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemi(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (15 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemi() {
    for (int i = 0; i < 15; i++) {
        __dst_alloc_buckets__dmemi[i].prev = 0;
        __dst_alloc_buckets__dmemi[i].next = 0;
    }
    __dst_alloc_list_push__dmemi(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemi(__dst_alloc_size_t request) {
    request = request / sizeof(int);
    if (request > (1 << 15)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemi(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemi(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemi(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemi(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemi(i);
            __dst_alloc_flip_parent_split__dmemi(i);
            bucket++;
            __dst_alloc_list_push__dmemi(bucket, __dst_alloc_index_for_node__dmemi(((i) + 1), bucket));
        }
        __dmemi[ptr]._link.prev = request;
        return ptr + 1;
    }
    return 0;
}
void __dst_alloc_free__dmemi(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemi(__dmemi[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemi(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemi(i);
        if (__dst_alloc_test_parent_split__dmemi(i)) break;
        __dst_alloc_list_remove__dmemi(bucket, __dst_alloc_index_for_node__dmemi(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemi(bucket, __dst_alloc_index_for_node__dmemi(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemi(unsigned long long );
void __dst_alloc_free__dmemi(unsigned long long );

struct btnode 
{
  int value;
  __didxL105R left;
  __didxL105R right;
}
;
typedef struct btnode node;
/* function declarations */

struct __dst_alloc_list__dmemL105R 
{
  struct __dst_alloc_list_base_t _link;
  node _data;
}
;
struct __dst_alloc_list__dmemL105R __dmemL105R[32768U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 54 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 73 "<stdin>"
unsigned char __dst_alloc_node_split__dmemL105R[(1 << (15 - 1)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemL105R(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemL105R[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemL105R(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemL105R[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemL105R(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemL105R[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemL105R[15] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemL105R(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 15 - 1;
    __dst_alloc_size_t size = 2;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemL105R(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemL105R[bucket].prev;
    __dmemL105R[entry]._link.prev = prev;
    __dmemL105R[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemL105R[bucket].next = entry;
    else __dmemL105R[prev]._link.next = entry;
    __dst_alloc_buckets__dmemL105R[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemL105R(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemL105R[entry]._link.prev;
    __dst_alloc_size_t next = __dmemL105R[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemL105R[bucket].next = next;
    else __dmemL105R[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemL105R[bucket].prev = prev;
    else __dmemL105R[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemL105R(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemL105R[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemL105R(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemL105R(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (15 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemL105R(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (15 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemL105R() {
    for (int i = 0; i < 15; i++) {
        __dst_alloc_buckets__dmemL105R[i].prev = 0;
        __dst_alloc_buckets__dmemL105R[i].next = 0;
    }
    __dst_alloc_list_push__dmemL105R(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemL105R(__dst_alloc_size_t request) {
    request = request / sizeof(::node);
    if (request > (1 << 15)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemL105R(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemL105R(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemL105R(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemL105R(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemL105R(i);
            __dst_alloc_flip_parent_split__dmemL105R(i);
            bucket++;
            __dst_alloc_list_push__dmemL105R(bucket, __dst_alloc_index_for_node__dmemL105R(((i) + 1), bucket));
        }
        __dmemL105R[ptr]._link.prev = request;
        return ptr + 1;
    }
    return 0;
}
void __dst_alloc_free__dmemL105R(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemL105R(__dmemL105R[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemL105R(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemL105R(i);
        if (__dst_alloc_test_parent_split__dmemL105R(i)) break;
        __dst_alloc_list_remove__dmemL105R(bucket, __dst_alloc_index_for_node__dmemL105R(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemL105R(bucket, __dst_alloc_index_for_node__dmemL105R(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemL105R(unsigned long long );
void __dst_alloc_free__dmemL105R(unsigned long long );
void insert(__didxL105R ,__didxL105R );
void dfs_traverse(__didxL105R );
/* global declarations */
__didxL105R root = 0L;
__didxi queue;
int front = 0;
int rear = - 1;
bool g_fallback = false;
extern "C" {

void process_top(int n,int *input,int *output,bool *fallback)
{
  
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=fallback offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=n bundle=control
  
#pragma HLS INTERFACE s_axilite port=input bundle=control
  
#pragma HLS INTERFACE s_axilite port=output bundle=control
  
#pragma HLS INTERFACE s_axilite port=fallback bundle=control
  
#pragma HLS INTERFACE s_axilite port=return bundle=control
  __didxL105R newitem = 0L;
  int num = 10;
  for (int i = 0; i < n; i++) {
    newitem = ((__didxL105R )(__dst_alloc_malloc__dmemL105R(sizeof(node ))));
    if (newitem == 0L) {
      g_fallback = true;
      goto fail;
    }
    (&(__dmemL105R + newitem + 0U - 1U) -> _data) -> left = (&(__dmemL105R + newitem + 0U - 1U) -> _data) -> right = 0L;
    (&(__dmemL105R + newitem + 0U - 1U) -> _data) -> value = input[i];
    if (root == 0L) {
      root = newitem;
    }
     else {
      insert(newitem,root);
      if (g_fallback) 
        goto fail;
    }
  }
  queue = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * n)));
  dfs_traverse(root);
  if (g_fallback) 
    goto fail;
  for (int i = 0; i < n; i++) 
    output[i] = (&(__dmemi + queue + i - 1U) -> _data)[0U];
  fail:
   *fallback = g_fallback;
}
}
/* inserting nodes of a tree */

struct __rect_packed_type_L5447R__L5448R 
{
  __didxL105R local0;
  __didxL105R local1;
  unsigned int _location;
}
;

void insert(__didxL105R newitem,__didxL105R root)
{
  volatile struct __rect_packed_type_L5447R__L5448R __rect_packed_var_L5447R__L5448R[64U];
  volatile unsigned int __rect_packed_top_L5447R__L5448R = 0U;
  __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location = 1U;
  __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local0 = newitem;
  __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 = root;
  ++__rect_packed_top_L5447R__L5448R;
  __rect_func_L0_L5447R__L5448R:
  if (0U == __rect_packed_top_L5447R__L5448R) 
    return ;
  --__rect_packed_top_L5447R__L5448R;
  if (1U == __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location) 
    goto __rect_func_L1_L5447R__L5448R;
  if (2U == __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location) 
    goto __rect_func_L2_L5447R__L5448R;
  if (3U == __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location) 
    goto __rect_func_L3_L5447R__L5448R;
  __rect_func_L1_L5447R__L5448R:
  if ((&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local0 + 0U - 1U) -> _data) -> value > (&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> value) {
    if ((&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> right == 0L) 
      (&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> right = __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local0;
     else {
      if (__rect_packed_top_L5447R__L5448R + 1U == 64U) {;
g_fallback = true;
        return ;
      }
      __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location = 2U;
      __rect_packed_var_L5447R__L5448R[1 + __rect_packed_top_L5447R__L5448R] . local1 = (&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> right;
      __rect_packed_var_L5447R__L5448R[1 + __rect_packed_top_L5447R__L5448R] . local0 = __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local0;
      ++__rect_packed_top_L5447R__L5448R;
      __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location = 1U;
      goto __rect_func_L1_L5447R__L5448R;
      __rect_func_L2_L5447R__L5448R:
      0;
    }
// newitem->value <= root->value
  }
   else {
    if ((&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> left == 0L) 
      (&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> left = __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local0;
     else {
      if (__rect_packed_top_L5447R__L5448R + 1U == 64U) {;
g_fallback = true;
        return ;
      }
      __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location = 3U;
      __rect_packed_var_L5447R__L5448R[1 + __rect_packed_top_L5447R__L5448R] . local1 = (&(__dmemL105R + __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local1 + 0U - 1U) -> _data) -> left;
      __rect_packed_var_L5447R__L5448R[1 + __rect_packed_top_L5447R__L5448R] . local0 = __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . local0;
      ++__rect_packed_top_L5447R__L5448R;
      __rect_packed_var_L5447R__L5448R[0 + __rect_packed_top_L5447R__L5448R] . _location = 1U;
      goto __rect_func_L1_L5447R__L5448R;
      __rect_func_L3_L5447R__L5448R:
      0;
    }
  }
  goto __rect_func_L0_L5447R__L5448R;
}
/* displaying elements using DFS traversal */

struct __rect_packed_type_L5449R__L5450R 
{
  __didxL105R local0;
  unsigned int _location;
}
;

void dfs_traverse(__didxL105R root)
{
  volatile struct __rect_packed_type_L5449R__L5450R __rect_packed_var_L5449R__L5450R[64U];
  volatile unsigned int __rect_packed_top_L5449R__L5450R = 0U;
  __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location = 1U;
  __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . local0 = root;
  ++__rect_packed_top_L5449R__L5450R;
  __rect_func_L0_L5449R__L5450R:
  if (0U == __rect_packed_top_L5449R__L5450R) 
    return ;
  --__rect_packed_top_L5449R__L5450R;
  if (1U == __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location) 
    goto __rect_func_L1_L5449R__L5450R;
  if (2U == __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location) 
    goto __rect_func_L2_L5449R__L5450R;
  if (3U == __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location) 
    goto __rect_func_L3_L5449R__L5450R;
  __rect_func_L1_L5449R__L5450R:
  if ((&(__dmemL105R + __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . local0 + 0U - 1U) -> _data) -> left != 0L) {
    if (__rect_packed_top_L5449R__L5450R + 1U == 64U) {;
g_fallback = true;
      return ;
    }
    __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location = 2U;
    __rect_packed_var_L5449R__L5450R[1 + __rect_packed_top_L5449R__L5450R] . local0 = (&(__dmemL105R + __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . local0 + 0U - 1U) -> _data) -> left;
    ++__rect_packed_top_L5449R__L5450R;
    __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location = 1U;
    goto __rect_func_L1_L5449R__L5450R;
    __rect_func_L2_L5449R__L5450R:
    0;
  }
  (&(__dmemi + queue + ++rear - 1U) -> _data)[0U] = (&(__dmemL105R + __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . local0 + 0U - 1U) -> _data) -> value;
  if ((&(__dmemL105R + __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . local0 + 0U - 1U) -> _data) -> right != 0L) {
    if (__rect_packed_top_L5449R__L5450R + 1U == 64U) {;
g_fallback = true;
      return ;
    }
    __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location = 3U;
    __rect_packed_var_L5449R__L5450R[1 + __rect_packed_top_L5449R__L5450R] . local0 = (&(__dmemL105R + __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . local0 + 0U - 1U) -> _data) -> right;
    ++__rect_packed_top_L5449R__L5450R;
    __rect_packed_var_L5449R__L5450R[0 + __rect_packed_top_L5449R__L5450R] . _location = 1U;
    goto __rect_func_L1_L5449R__L5450R;
    __rect_func_L3_L5449R__L5450R:
    0;
  }
  goto __rect_func_L0_L5449R__L5450R;
}
