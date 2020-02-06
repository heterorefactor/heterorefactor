
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
const char *__dst_filename = "/tmp/hetero-profile";
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
struct __dst_alloc_list__dmemi __dmemi[1025U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 27 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 47 "<stdin>"
__dst_alloc_size_t __dst_alloc_malloc__dmemi(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(int);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemi"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemi *allocated = (__dst_alloc_list__dmemi *)malloc(
            request * sizeof(__dst_alloc_list__dmemi));
    allocated[0]._link.prev = request;
    return allocated - __dmemi;
}
void __dst_alloc_free__dmemi(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemi[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemi"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemi + ptr);
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
struct __dst_alloc_list__dmemL105R __dmemL105R[1025U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 27 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 47 "<stdin>"
__dst_alloc_size_t __dst_alloc_malloc__dmemL105R(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(::node);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemL105R"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemL105R *allocated = (__dst_alloc_list__dmemL105R *)malloc(
            request * sizeof(__dst_alloc_list__dmemL105R));
    allocated[0]._link.prev = request;
    return allocated - __dmemL105R;
}
void __dst_alloc_free__dmemL105R(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemL105R[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemL105R"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemL105R + ptr);
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

void process_top(int n,int *input,int *output,int *fallback)
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
    (&(__dmemL105R + newitem + 0U) -> _data) -> left = (&(__dmemL105R + newitem + 0U) -> _data) -> right = 0L;
    (&(__dmemL105R + newitem + 0U) -> _data) -> value = input[i];
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
    output[i] = (&(__dmemi + queue + i) -> _data)[0U];
  fail:
   *fallback = g_fallback;
}
}
/* inserting nodes of a tree */

void insert(__didxL105R newitem,__didxL105R root)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5447R__L5448R\n");fflush((FILE *)__dst_file);
  if ((&(__dmemL105R + newitem + 0U) -> _data) -> value > (&(__dmemL105R + root + 0U) -> _data) -> value) {
    if ((&(__dmemL105R + root + 0U) -> _data) -> right == 0L) 
      (&(__dmemL105R + root + 0U) -> _data) -> right = newitem;
     else 
      insert(newitem,(&(__dmemL105R + root + 0U) -> _data) -> right);
// newitem->value <= root->value
  }
   else {
    if ((&(__dmemL105R + root + 0U) -> _data) -> left == 0L) 
      (&(__dmemL105R + root + 0U) -> _data) -> left = newitem;
     else 
      insert(newitem,(&(__dmemL105R + root + 0U) -> _data) -> left);
  }
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5447R__L5448R\n");fflush((FILE *)__dst_file);
}
/* displaying elements using DFS traversal */

void dfs_traverse(__didxL105R root)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5449R__L5450R\n");fflush((FILE *)__dst_file);
  if ((&(__dmemL105R + root + 0U) -> _data) -> left != 0L) {
    dfs_traverse((&(__dmemL105R + root + 0U) -> _data) -> left);
  }
  (&(__dmemi + queue + ++rear) -> _data)[0U] = (&(__dmemL105R + root + 0U) -> _data) -> value;
  if ((&(__dmemL105R + root + 0U) -> _data) -> right != 0L) {
    dfs_traverse((&(__dmemL105R + root + 0U) -> _data) -> right);
  }
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5449R__L5450R\n");fflush((FILE *)__dst_file);
}
