
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
const char *__dst_filename = "/tmp/hetero-profile";
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
struct __dst_alloc_list__dmemL106R __dmemL106R[1024U];
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
__dst_alloc_size_t __dst_alloc_malloc__dmemL106R(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(::NODE);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemL106R"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemL106R *allocated = (__dst_alloc_list__dmemL106R *)malloc(
            request * sizeof(__dst_alloc_list__dmemL106R));
    allocated[0]._link.prev = request;
    return allocated - __dmemL106R + 1;
}
void __dst_alloc_free__dmemL106R(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemL106R[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemL106R"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemL106R + ptr);
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

__didxL106R reverse_rec(__didxL106R ptr,__didxL106R previous)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5458R__L5459R\n");fflush((FILE *)__dst_file);
  __didxL106R temp;
  if ((&(__dmemL106R + ptr + 0U - 1U) -> _data) -> next == 0L) {
    (&(__dmemL106R + ptr + 0U - 1U) -> _data) -> next = previous;
    __didxL106R __temp0__ = ptr;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5458R__L5459R\n");fflush((FILE *)__dst_file);
    0;
    return __temp0__;
  }
   else {
    temp = reverse_rec((&(__dmemL106R + ptr + 0U - 1U) -> _data) -> next,ptr);
    (&(__dmemL106R + ptr + 0U - 1U) -> _data) -> next = previous;
    __didxL106R __temp1__ = temp;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5458R__L5459R\n");fflush((FILE *)__dst_file);
    0;
    return __temp1__;
  }
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5458R__L5459R\n");fflush((FILE *)__dst_file);
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
