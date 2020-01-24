
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxL106R was ::NODE*
typedef unsigned long long __didxL106R;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;
// === END REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
bool g_fallback = false;
typedef struct {
int info;}DATA;
typedef struct node {
DATA data;
__didxL106R next;}NODE;

struct __dst_alloc_list__dmemL106R 
{
  struct __dst_alloc_list_base_t _link;
  NODE _data;
}
;
struct __dst_alloc_list__dmemL106R __dmemL106R[4096U];
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
unsigned char __dst_alloc_node_split__dmemL106R[(1 << (12 - 1)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemL106R(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemL106R[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemL106R(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemL106R[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemL106R(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemL106R[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemL106R[12] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemL106R(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 12 - 1;
    __dst_alloc_size_t size = 2;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemL106R(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemL106R[bucket].prev;
    __dmemL106R[entry]._link.prev = prev;
    __dmemL106R[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemL106R[bucket].next = entry;
    else __dmemL106R[prev]._link.next = entry;
    __dst_alloc_buckets__dmemL106R[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemL106R(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemL106R[entry]._link.prev;
    __dst_alloc_size_t next = __dmemL106R[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemL106R[bucket].next = next;
    else __dmemL106R[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemL106R[bucket].prev = prev;
    else __dmemL106R[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemL106R(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemL106R[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemL106R(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemL106R(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (12 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemL106R(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (12 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemL106R() {
    for (int i = 0; i < 12; i++) {
        __dst_alloc_buckets__dmemL106R[i].prev = 0;
        __dst_alloc_buckets__dmemL106R[i].next = 0;
    }
    __dst_alloc_list_push__dmemL106R(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemL106R(__dst_alloc_size_t request) {
    request = request / sizeof(::NODE);
    if (request > (1 << 12)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemL106R(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemL106R(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemL106R(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemL106R(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemL106R(i);
            __dst_alloc_flip_parent_split__dmemL106R(i);
            bucket++;
            __dst_alloc_list_push__dmemL106R(bucket, __dst_alloc_index_for_node__dmemL106R(((i) + 1), bucket));
        }
        __dmemL106R[ptr]._link.prev = request;
        return ptr + 1;
    }
    return 0;
}
void __dst_alloc_free__dmemL106R(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemL106R(__dmemL106R[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemL106R(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemL106R(i);
        if (__dst_alloc_test_parent_split__dmemL106R(i)) break;
        __dst_alloc_list_remove__dmemL106R(bucket, __dst_alloc_index_for_node__dmemL106R(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemL106R(bucket, __dst_alloc_index_for_node__dmemL106R(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemL106R(unsigned long long );
void __dst_alloc_free__dmemL106R(unsigned long long );

void init(__didxL106R *head)
{
   *head = 0L;
}

int *output_list(__didxL106R head,int *curr)
{
  __didxL106R temp;
  for (temp = head; temp; temp = (&(__dmemL106R + temp + 0U - 1U) -> _data) -> next) {
     *(curr++) = (&(__dmemL106R + temp + 0U - 1U) -> _data) -> data . info;
  }
  return curr;
}

__didxL106R add(__didxL106R node,DATA data)
{
  __didxL106R temp = (__didxL106R )(__dst_alloc_malloc__dmemL106R(sizeof(NODE )));
  if (temp == 0L) {
    g_fallback = true;
    return 0L;
  }
  (&(__dmemL106R + temp + 0U - 1U) -> _data) -> data = data;
  (&(__dmemL106R + temp + 0U - 1U) -> _data) -> next = node;
  node = temp;
  return node;
}

void add_at(__didxL106R node,DATA data)
{
  __didxL106R temp = (__didxL106R )(__dst_alloc_malloc__dmemL106R(sizeof(NODE )));
  if (temp == 0L) {
    g_fallback = true;
    return ;
  }
  (&(__dmemL106R + temp + 0U - 1U) -> _data) -> data = data;
  (&(__dmemL106R + temp + 0U - 1U) -> _data) -> next = (&(__dmemL106R + node + 0U - 1U) -> _data) -> next;
  (&(__dmemL106R + node + 0U - 1U) -> _data) -> next = temp;
}

void remove_node(__didxL106R head)
{
  __didxL106R temp = (&(__dmemL106R + head + 0U - 1U) -> _data) -> next;
  (&(__dmemL106R + head + 0U - 1U) -> _data) -> next = (&(__dmemL106R + (&(__dmemL106R + head + 0U - 1U) -> _data) -> next + 0U - 1U) -> _data) -> next;
  __dst_alloc_free__dmemL106R(temp);
}

struct __rect_packed_type_L5458R__L5459R 
{
  __didxL106R local0;
  __didxL106R local1;
  __didxL106R local2;
  unsigned int _location;
  __didxL106R _return;
  __didxL106R _ret0;
}
;

__didxL106R reverse_rec(__didxL106R ptr,__didxL106R previous)
{
  volatile struct __rect_packed_type_L5458R__L5459R __rect_packed_var_L5458R__L5459R[2048U];
  volatile unsigned int __rect_packed_top_L5458R__L5459R = 0U;
  __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _location = 1U;
  __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0 = ptr;
  __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local1 = previous;
  ++__rect_packed_top_L5458R__L5459R;
  __rect_func_L0_L5458R__L5459R:
  if (0U == __rect_packed_top_L5458R__L5459R) 
    return __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _return;
  --__rect_packed_top_L5458R__L5459R;
  if (1U == __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _location) 
    goto __rect_func_L1_L5458R__L5459R;
  if (2U == __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _location) 
    goto __rect_func_L2_L5458R__L5459R;
  __rect_func_L1_L5458R__L5459R:
  __didxL106R temp;
  if ((&(__dmemL106R + __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0 + 0U - 1U) -> _data) -> next == 0L) {
    (&(__dmemL106R + __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0 + 0U - 1U) -> _data) -> next = __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local1;
    __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _return = __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0;
    goto __rect_func_L0_L5458R__L5459R;
  }
   else {
    if (__rect_packed_top_L5458R__L5459R + 1U == 2048U) {;
g_fallback = true;
      __didxL106R __temp0__;
      return __temp0__;
    }
    __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _location = 2U;
    __rect_packed_var_L5458R__L5459R[1 + __rect_packed_top_L5458R__L5459R] . local1 = __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0;
    __rect_packed_var_L5458R__L5459R[1 + __rect_packed_top_L5458R__L5459R] . local0 = (&(__dmemL106R + __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0 + 0U - 1U) -> _data) -> next;
    ++__rect_packed_top_L5458R__L5459R;
    __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _location = 1U;
    goto __rect_func_L1_L5458R__L5459R;
    __rect_func_L2_L5458R__L5459R:
    __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . __rect_packed_type_L5458R__L5459R::_ret0 = (__rect_packed_var_L5458R__L5459R[1 + __rect_packed_top_L5458R__L5459R] . _return);
    __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local2 = __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . __rect_packed_type_L5458R__L5459R::_ret0;
    (&(__dmemL106R + __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local0 + 0U - 1U) -> _data) -> next = __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local1;
    __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . _return = __rect_packed_var_L5458R__L5459R[0 + __rect_packed_top_L5458R__L5459R] . local2;
    goto __rect_func_L0_L5458R__L5459R;
  }
  goto __rect_func_L0_L5458R__L5459R;
}

__didxL106R sort_list(__didxL106R head)
{
  __didxL106R tmpPtr = head;
  __didxL106R tmpNxt = (&(__dmemL106R + head + 0U - 1U) -> _data) -> next;
  DATA tmp;
  while(tmpNxt != 0L){
    while(tmpNxt != tmpPtr){
      if ((&(__dmemL106R + tmpNxt + 0U - 1U) -> _data) -> data . info < (&(__dmemL106R + tmpPtr + 0U - 1U) -> _data) -> data . info) {
        tmp = (&(__dmemL106R + tmpPtr + 0U - 1U) -> _data) -> data;
        (&(__dmemL106R + tmpPtr + 0U - 1U) -> _data) -> data = (&(__dmemL106R + tmpNxt + 0U - 1U) -> _data) -> data;
        (&(__dmemL106R + tmpNxt + 0U - 1U) -> _data) -> data = tmp;
      }
      tmpPtr = (&(__dmemL106R + tmpPtr + 0U - 1U) -> _data) -> next;
    }
    tmpPtr = head;
    tmpNxt = (&(__dmemL106R + tmpNxt + 0U - 1U) -> _data) -> next;
  }
  return tmpPtr;
}
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
  __didxL106R head;
  __didxL106R node;
  DATA element;
  int *curr;
  init(&head);
  for (int i = 0; i < n; i++) {
    element . info = input[i];
    head = add(head,element);
    if (!head) 
      goto fail;
  }
  curr = output;
  curr = output_list(head,curr);
   *(curr++) = - 1;
  node = (&(__dmemL106R + (&(__dmemL106R + (&(__dmemL106R + head + 0U - 1U) -> _data) -> next + 0U - 1U) -> _data) -> next + 0U - 1U) -> _data) -> next;
  element . info = 2000;
  add_at(node,element);
  if (g_fallback) 
    goto fail;
  curr = output_list(head,curr);
   *(curr++) = - 1;
  node = (&(__dmemL106R + (&(__dmemL106R + head + 0U - 1U) -> _data) -> next + 0U - 1U) -> _data) -> next;
  remove_node(node);
  head = sort_list(head);
  curr = output_list(head,curr);
   *(curr++) = - 1;
  head = reverse_rec(head,0L);
  if (g_fallback) 
    goto fail;
  curr = output_list(head,curr);
   *(curr++) = - 1;
  fail:
   *fallback = g_fallback;
}
}
