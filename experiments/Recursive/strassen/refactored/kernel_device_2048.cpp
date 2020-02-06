
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxi was int *
typedef unsigned long long __didxi;
// type __didx__Pb____Pb__i__Pe____Pe__ was int ***
typedef unsigned long long __didx__Pb____Pb__i__Pe____Pe__;
// type __didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ was int ****
typedef unsigned long long __didx__Pb____Pb____Pb__i__Pe____Pe____Pe__;
// type __didx__Pb__i__Pe__ was int **
typedef unsigned long long __didx__Pb__i__Pe__;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;

struct __dst_alloc_list__dmemi 
{
  struct __dst_alloc_list_base_t _link;
  int _data;
}
;
struct __dst_alloc_list__dmemi __dmemi[4097U];
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
unsigned char __dst_alloc_node_split__dmemi[(1 << (12)) / 8];
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
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemi[12 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemi(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 12;
    __dst_alloc_size_t size = 1;
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
    return 1 + ((bucket_index - (1 << bucket) + 1) << (12 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemi(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (12 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemi() {
    for (int i = 0; i <= 12; i++) {
        __dst_alloc_buckets__dmemi[i].prev = 0;
        __dst_alloc_buckets__dmemi[i].next = 0;
    }
    __dst_alloc_list_push__dmemi(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemi(__dst_alloc_size_t request) {
    request = request / sizeof(int);
    if (request > (1 << 12)) return 0;
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
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemi(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
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

struct __dst_alloc_list__dmemUL 
{
  struct __dst_alloc_list_base_t _link;
  unsigned long long _data;
}
;
struct __dst_alloc_list__dmemUL __dmemUL[1025U];
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
unsigned char __dst_alloc_node_split__dmemUL[(1 << (10)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemUL(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemUL[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemUL(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemUL[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemUL(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemUL[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemUL[10 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemUL(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 10;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemUL(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemUL[bucket].prev;
    __dmemUL[entry]._link.prev = prev;
    __dmemUL[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemUL[bucket].next = entry;
    else __dmemUL[prev]._link.next = entry;
    __dst_alloc_buckets__dmemUL[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemUL(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemUL[entry]._link.prev;
    __dst_alloc_size_t next = __dmemUL[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemUL[bucket].next = next;
    else __dmemUL[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemUL[bucket].prev = prev;
    else __dmemUL[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemUL(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemUL[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemUL(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemUL(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (10 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemUL(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (10 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemUL() {
    for (int i = 0; i <= 10; i++) {
        __dst_alloc_buckets__dmemUL[i].prev = 0;
        __dst_alloc_buckets__dmemUL[i].next = 0;
    }
    __dst_alloc_list_push__dmemUL(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemUL(__dst_alloc_size_t request) {
    request = request / sizeof(unsigned long long);
    if (request > (1 << 10)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemUL(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemUL(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemUL(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemUL(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemUL(i);
            __dst_alloc_flip_parent_split__dmemUL(i);
            bucket++;
            __dst_alloc_list_push__dmemUL(bucket, __dst_alloc_index_for_node__dmemUL(((i) + 1), bucket));
        }
        __dmemUL[ptr]._link.prev = request;
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemUL(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemUL(__dmemUL[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemUL(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemUL(i);
        if (__dst_alloc_test_parent_split__dmemUL(i)) break;
        __dst_alloc_list_remove__dmemUL(bucket, __dst_alloc_index_for_node__dmemUL(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemUL(bucket, __dst_alloc_index_for_node__dmemUL(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemUL(unsigned long long );
void __dst_alloc_free__dmemUL(unsigned long long );
// === END REC SUPPORT LIBRARY ===
// CPP program to implement Strassen’s Matrix
// Multiplication Algorithm
// from https://www.geeksforgeeks.org/strassens-matrix-multiplication-algorithm-implementation/
#include <cstdlib>
bool g_fallback = false;
__didx__Pb__i__Pe__ Strassen(__didx__Pb__i__Pe__ a,__didx__Pb__i__Pe__ b,int n,int l,int m);
extern "C" {

void process_top(int *np,int *lp,int *mp,int *mat1,int *mat2,int *mat3,int *fallback)
{
  
#pragma HLS INTERFACE m_axi port=np offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=lp offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=mp offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=mat1 offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=mat2 offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=mat3 offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=fallback offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=np bundle=control
  
#pragma HLS INTERFACE s_axilite port=lp bundle=control
  
#pragma HLS INTERFACE s_axilite port=mp bundle=control
  
#pragma HLS INTERFACE s_axilite port=mat1 bundle=control
  
#pragma HLS INTERFACE s_axilite port=mat2 bundle=control
  
#pragma HLS INTERFACE s_axilite port=mat3 bundle=control
  
#pragma HLS INTERFACE s_axilite port=fallback bundle=control
  
#pragma HLS INTERFACE s_axilite port=return bundle=control
  int n =  *np;
  int l =  *lp;
  int m =  *mp;
  __didx__Pb__i__Pe__ matA = (__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * n));
  __didx__Pb__i__Pe__ matB;
  __didx__Pb__i__Pe__ matC;
  if (!matA) {
    g_fallback = true;
    goto fail;
  }
  for (int i = 0; i < n; i++) {
    (&(__dmemUL + matA + i) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * l)));
    if (!(&(__dmemUL + matA + i) -> _data)[0U]) {
      g_fallback = true;
      goto fail;
    }
    for (int j = 0; j < l; j++) 
      (&(__dmemi + (&(__dmemUL + matA + i) -> _data)[0U] + j) -> _data)[0U] = mat1[i * l + j];
  }
  matB = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * l)));
  if (!matB) {
    g_fallback = true;
    goto fail;
  }
  for (int i = 0; i < l; i++) {
    (&(__dmemUL + matB + i) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * m)));
    if (!(&(__dmemUL + matB + i) -> _data)[0U]) {
      g_fallback = true;
      goto fail;
    }
    for (int j = 0; j < m; j++) 
      (&(__dmemi + (&(__dmemUL + matB + i) -> _data)[0U] + j) -> _data)[0U] = mat2[i * m + j];
  }
  matC = Strassen(matA,matB,n,l,m);
  if (g_fallback) 
    goto fail;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      mat3[i * m + j] = (&(__dmemi + (&(__dmemUL + matC + i) -> _data)[0U] + j) -> _data)[0U];
    }
    __dst_alloc_free__dmemi((&(__dmemUL + matC + i) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(matC);
  fail:
   *fallback = g_fallback;
}
}
// Strassen's Algorithm for matrix multiplication
// Complexity: O(n^2.808)

__didx__Pb__i__Pe__ MatrixMultiply(__didx__Pb__i__Pe__ a,__didx__Pb__i__Pe__ b,int n,int l,int m)
{
  __didx__Pb__i__Pe__ c = (__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * n));
  if (!c) {
    g_fallback = true;
    return 0L;
  }
  for (int i = 0; i < n; i++) {
    (&(__dmemUL + c + i) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * m)));
    if (!(&(__dmemUL + c + i) -> _data)[0U]) {
      g_fallback = true;
      return 0L;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      (&(__dmemi + (&(__dmemUL + c + i) -> _data)[0U] + j) -> _data)[0U] = 0;
      for (int k = 0; k < l; k++) {
        (&(__dmemi + (&(__dmemUL + c + i) -> _data)[0U] + j) -> _data)[0U] += (&(__dmemi + (&(__dmemUL + a + i) -> _data)[0U] + k) -> _data)[0U] * (&(__dmemi + (&(__dmemUL + b + k) -> _data)[0U] + j) -> _data)[0U];
      }
    }
  }
  return c;
}

struct __rect_packed_type_L5457R__L5458R 
{
  __didx__Pb__i__Pe__ local0;
  __didx__Pb__i__Pe__ local1;
  int local2;
  int local3;
  int local4;
  __didx__Pb__i__Pe__ local5;
  int local6;
  int local7;
  int local8;
  int local9;
  __didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ local10;
  int local11;
  int local12;
  int local13;
  int local14;
  int local15;
  int local16;
  __didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ local17;
  int local18;
  int local19;
  int local20;
  int local21;
  int local22;
  int local23;
  __didx__Pb____Pb__i__Pe____Pe__ local24;
  int local25;
  int local26;
  int local27;
  int local28;
  int local29;
  int local30;
  int local31;
  int local32;
  int local33;
  int local34;
  int local35;
  int local36;
  int local37;
  int local38;
  int local39;
  int local40;
  int local41;
  int local42;
  int local43;
  int local44;
  int local45;
  __didx__Pb____Pb__i__Pe____Pe__ local46;
  int local47;
  int local48;
  int local49;
  int local50;
  int local51;
  int local52;
  int local53;
  int local54;
  int local55;
  int local56;
  int local57;
  int local58;
  int local59;
  unsigned int _location;
  __didx__Pb__i__Pe__ _return;
  __didx__Pb__i__Pe__ _ret0;
  __didx__Pb__i__Pe__ _ret1;
  __didx__Pb__i__Pe__ _ret2;
  __didx__Pb__i__Pe__ _ret3;
  __didx__Pb__i__Pe__ _ret4;
  __didx__Pb__i__Pe__ _ret5;
  __didx__Pb__i__Pe__ _ret6;
}
;

__didx__Pb__i__Pe__ Strassen(__didx__Pb__i__Pe__ a,__didx__Pb__i__Pe__ b,int n,int l,int m)
{
  volatile struct __rect_packed_type_L5457R__L5458R __rect_packed_var_L5457R__L5458R[8U];
  volatile unsigned int __rect_packed_top_L5457R__L5458R = 0U;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local0 = a;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local1 = b;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 = n;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3 = l;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4 = m;
  ++__rect_packed_top_L5457R__L5458R;
  __rect_func_L0_L5457R__L5458R:
  if (0U == __rect_packed_top_L5457R__L5458R) 
    return __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return;
  --__rect_packed_top_L5457R__L5458R;
  if (1U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L1_L5457R__L5458R;
  if (2U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L2_L5457R__L5458R;
  if (3U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L3_L5457R__L5458R;
  if (4U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L4_L5457R__L5458R;
  if (5U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L5_L5457R__L5458R;
  if (6U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L6_L5457R__L5458R;
  if (7U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L7_L5457R__L5458R;
  if (8U == __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location) 
    goto __rect_func_L8_L5457R__L5458R;
  __rect_func_L1_L5457R__L5458R:
  if (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 == 1 || __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3 == 1 || __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4 == 1) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = MatrixMultiply(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local0,__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local1,__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2,__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3,__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4);
    goto __rect_func_L0_L5457R__L5458R;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2)));
  if (!__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5) {
    g_fallback = true;
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local6 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local6 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local6++) {
    (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local6) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4)));
    if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local6) -> _data)[0U]) {
      g_fallback = true;
      __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
      goto __rect_func_L0_L5457R__L5458R;
    }
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7 = (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 >> 1) + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 & 1);
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8 = (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3 >> 1) + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3 & 1);
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9 = (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4 >> 1) + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4 & 1);
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 = ((__didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int ***) * 2)));
  if (!__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10) {
    g_fallback = true;
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11++) {
    (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U] = ((__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 2)));
    if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U]) {
      g_fallback = true;
      __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
      goto __rect_func_L0_L5457R__L5458R;
    }
    for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12++) {
      (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13++) {
        (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
        if (!(&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local14 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local14 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local14++) {
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local15 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13 + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11 & 1) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local16 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local14 + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12 & 1) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local11) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local12) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local13) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local14) -> _data)[0U] = (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local15 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 && __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local16 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3?(&(__dmemi + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local0 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local15) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local16) -> _data)[0U] : 0);
        }
      }
    }
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 = ((__didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int ***) * 2)));
  if (!__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17) {
    g_fallback = true;
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18++) {
    (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U] = ((__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 2)));
    if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U]) {
      g_fallback = true;
      __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
      goto __rect_func_L0_L5457R__L5458R;
    }
    for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19++) {
      (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20++) {
        (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)));
        if (!(&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local21 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local21 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local21++) {
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local22 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20 + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18 & 1) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local23 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local21 + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19 & 1) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local18) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local19) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local20) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local21) -> _data)[0U] = (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local22 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local3 && __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local23 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4?(&(__dmemi + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local1 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local22) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local23) -> _data)[0U] : 0);
        }
      }
    }
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 = ((__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 10)));
  if (!__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24) {
    g_fallback = true;
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25 < 10; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25++) {
    switch(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25){
      case 0:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local27 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local27 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local27++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local27) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 0) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local27) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 1) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local26) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local27) -> _data)[0U];
        }
      }
      break; 
      case 1:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local29 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local29 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local29++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local29) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 0) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local29) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 0) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local28) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local29) -> _data)[0U];
        }
      }
      break; 
      case 2:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local31 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local31 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local31++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local31) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 1) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local31) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 1) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local30) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local31) -> _data)[0U];
        }
      }
      break; 
      case 3:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local33 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local33 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local33++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local33) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 1) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local33) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 0) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local32) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local33) -> _data)[0U];
        }
      }
      break; 
      case 4:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local35 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local35 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local35++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local35) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 0) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local35) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 1) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local34) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local35) -> _data)[0U];
        }
      }
      break; 
      case 5:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local37 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local37 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local37++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local37) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 0) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local37) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 1) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local36) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local37) -> _data)[0U];
        }
      }
      break; 
      case 6:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local39 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local39 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local39++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local39) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 0) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local39) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 1) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local38) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local39) -> _data)[0U];
        }
      }
      break; 
      case 7:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local41 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local41 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local41++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local41) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 1) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local41) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 1) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local40) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local41) -> _data)[0U];
        }
      }
      break; 
      case 8:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local43 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local43 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local43++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local43) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 0) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local43) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 1) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local42) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local43) -> _data)[0U];
        }
      }
      break; 
      case 9:
      (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8)));
      if (!(&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U]) {
        g_fallback = true;
        __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
        goto __rect_func_L0_L5457R__L5458R;
      }
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44++) {
        (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)));
        if (!(&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44) -> _data)[0U]) {
          g_fallback = true;
          __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
          goto __rect_func_L0_L5457R__L5458R;
        }
        for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local45 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local45 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local45++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local25) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local45) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 0) -> _data)[0U] + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local45) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 0) -> _data)[0U] + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local44) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local45) -> _data)[0U];
        }
      }
      break; 
    }
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 = ((__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 7)));
  if (!__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46) {
    g_fallback = true;
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp0__;
    return __temp0__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 2U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 0) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 0) -> _data)[0U] + 0) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L2_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret0 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 0) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret0;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp1__;
    return __temp1__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 3U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 1) -> _data)[0U] + 1) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 1) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L3_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret1 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 1) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret1;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp2__;
    return __temp2__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 4U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + 0) -> _data)[0U] + 0) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 2) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L4_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret2 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 2) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret2;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp3__;
    return __temp3__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 5U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 3) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + 1) -> _data)[0U] + 1) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L5_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret3 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 3) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret3;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp4__;
    return __temp4__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 6U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 5) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 4) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L6_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret4 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 4) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret4;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp5__;
    return __temp5__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 7U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 7) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 6) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L7_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret5 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 5) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret5;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  if (__rect_packed_top_L5457R__L5458R + 1U == 8U) {;
g_fallback = true;
    __didx__Pb__i__Pe__ __temp6__;
    return __temp6__;
  }
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 8U;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local4 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local3 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local2 = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7;
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local1 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 9) -> _data)[0U];
  __rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . local0 = (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + 8) -> _data)[0U];
  ++__rect_packed_top_L5457R__L5458R;
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _location = 1U;
  goto __rect_func_L1_L5457R__L5458R;
  __rect_func_L8_L5457R__L5458R:
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret6 = (__rect_packed_var_L5457R__L5458R[1 + __rect_packed_top_L5457R__L5458R] . _return);
  (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 6) -> _data)[0U] = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . __rect_packed_type_L5457R__L5458R::_ret6;
  if (g_fallback) {
    __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = 0L;
    goto __rect_func_L0_L5457R__L5458R;
  }
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47++) {
    for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48++) {
      (&(__dmemi + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 4) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 3) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 5) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U];
      if (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4) 
        (&(__dmemi + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 1) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U];
      if (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2) 
        (&(__dmemi + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 2) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 3) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U];
      if (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 && __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local4) 
        (&(__dmemi + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5 + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7)) -> _data)[0U] + (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local9)) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 4) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 0) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 2) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + 6) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local47) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local48) -> _data)[0U];
    }
  }
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local49 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local49 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local49++) {
    for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local50 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local50 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local50++) {
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local51 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local51 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local51++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local49) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local50) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local51) -> _data)[0U]);
      }
      __dst_alloc_free__dmemUL((&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local49) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local50) -> _data)[0U]);
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local49) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local10);
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local52 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local52 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local52++) {
    for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local53 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local53 < 2; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local53++) {
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local54 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local54 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local54++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local52) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local53) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local54) -> _data)[0U]);
      }
      __dst_alloc_free__dmemUL((&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local52) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local53) -> _data)[0U]);
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local52) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local17);
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55 < 10; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55++) {
    switch(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55){
      case 0:;
      case 3:;
      case 5:;
      case 7:;
      case 9:
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local56 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local56 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local8; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local56++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local56) -> _data)[0U]);
      }
      break; 
      case 1:;
      case 2:;
      case 4:;
      case 6:;
      case 8:
      for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local57 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local57 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local57++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local57) -> _data)[0U]);
      }
      break; 
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local55) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local24);
  for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local58 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local58 < 7; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local58++) {
    for (__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local59 = 0; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local59 < __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local2 >> 1; __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local59++) {
      __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local58) -> _data)[0U] + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local59) -> _data)[0U]);
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46 + __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local58) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(__rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local46);
  __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . _return = __rect_packed_var_L5457R__L5458R[0 + __rect_packed_top_L5457R__L5458R] . local5;
  goto __rect_func_L0_L5457R__L5458R;
  goto __rect_func_L0_L5457R__L5458R;
}
