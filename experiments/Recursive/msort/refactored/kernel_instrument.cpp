
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxclass_Node was struct ::Node{int data;struct Node *next;}*
typedef unsigned long long __didxclass_Node;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "/tmp/hetero-profile";
unsigned long long __dst_file = 0;
// === END REC SUPPORT LIBRARY ===
// C code for linked list merged sort
#include <stdlib.h>
bool g_fallback = false;
/* Link list node */

struct Node 
{
  int data;
  __didxclass_Node next;
}
;
/* function prototypes */

struct __dst_alloc_list__dmemclass_Node 
{
  struct __dst_alloc_list_base_t _link;
  struct Node _data;
}
;
struct __dst_alloc_list__dmemclass_Node __dmemclass_Node[1024U];
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
__dst_alloc_size_t __dst_alloc_malloc__dmemclass_Node(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(struct ::Node);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemclass_Node"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemclass_Node *allocated = (__dst_alloc_list__dmemclass_Node *)malloc(
            request * sizeof(__dst_alloc_list__dmemclass_Node));
    allocated[0]._link.prev = request;
    return allocated - __dmemclass_Node + 1;
}
void __dst_alloc_free__dmemclass_Node(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemclass_Node[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemclass_Node"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemclass_Node + ptr);
}
unsigned long long __dst_alloc_malloc__dmemclass_Node(unsigned long long );
void __dst_alloc_free__dmemclass_Node(unsigned long long );
__didxclass_Node SortedMerge(__didxclass_Node a,__didxclass_Node b);
/* sorts the linked list by changing next pointers (not data) */

__didxclass_Node MergeSort(__didxclass_Node head)
{
/* Base case -- length 0 or 1 */
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5448R__L5449R\n");fflush((FILE *)__dst_file);
  if (head == 0L || (&(__dmemclass_Node + head + 0U - 1U) -> _data) -> next == 0L) {
    __didxclass_Node __temp0__ = head;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5448R__L5449R\n");fflush((FILE *)__dst_file);
    0;
    return __temp0__;
  }
  __didxclass_Node fast = (&(__dmemclass_Node + head + 0U - 1U) -> _data) -> next;
  __didxclass_Node slow = head;
/* Advance 'fast' two nodes, and advance 'slow' one node */
  while(fast != 0L){
    fast = (&(__dmemclass_Node + fast + 0U - 1U) -> _data) -> next;
    if (fast != 0L) {
      slow = (&(__dmemclass_Node + slow + 0U - 1U) -> _data) -> next;
      fast = (&(__dmemclass_Node + fast + 0U - 1U) -> _data) -> next;
    }
  }
/* 'slow' is before the midpoint in the list, so split it in two
  at that point. */
  __didxclass_Node a = head;
  __didxclass_Node b = (&(__dmemclass_Node + slow + 0U - 1U) -> _data) -> next;
  (&(__dmemclass_Node + slow + 0U - 1U) -> _data) -> next = 0L;
/* Recursively sort the sublists */
  a = MergeSort(a);
  b = MergeSort(b);
/* answer = merge the two sorted lists together */
  __didxclass_Node __temp1__ = SortedMerge(a,b);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5448R__L5449R\n");fflush((FILE *)__dst_file);
  0;
  return __temp1__;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5448R__L5449R\n");fflush((FILE *)__dst_file);
}
/* See https://www.geeksforgeeks.org/?p=3622 for details of this
function */

__didxclass_Node SortedMerge(__didxclass_Node a,__didxclass_Node b)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5446R__L5447R\n");fflush((FILE *)__dst_file);
  __didxclass_Node result = 0L;
/* Base cases */
  if (a == 0L) {
    __didxclass_Node __temp2__ = b;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5446R__L5447R\n");fflush((FILE *)__dst_file);
    0;
    return __temp2__;
  }
   else if (b == 0L) {
    __didxclass_Node __temp3__ = a;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5446R__L5447R\n");fflush((FILE *)__dst_file);
    0;
    return __temp3__;
  }
/* Pick either a or b, and recur */
  if ((&(__dmemclass_Node + a + 0U - 1U) -> _data) -> data <= (&(__dmemclass_Node + b + 0U - 1U) -> _data) -> data) {
    result = a;
    (&(__dmemclass_Node + result + 0U - 1U) -> _data) -> next = SortedMerge((&(__dmemclass_Node + a + 0U - 1U) -> _data) -> next,b);
  }
   else {
    result = b;
    (&(__dmemclass_Node + result + 0U - 1U) -> _data) -> next = SortedMerge(a,(&(__dmemclass_Node + b + 0U - 1U) -> _data) -> next);
  }
  __didxclass_Node __temp4__ = result;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5446R__L5447R\n");fflush((FILE *)__dst_file);
  0;
  return __temp4__;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5446R__L5447R\n");fflush((FILE *)__dst_file);
}
/* Function to print nodes in a given linked list */

int *printList(__didxclass_Node node,int *output)
{
  while(node != 0L){
     *(output++) = (&(__dmemclass_Node + node + 0U - 1U) -> _data) -> data;
    node = (&(__dmemclass_Node + node + 0U - 1U) -> _data) -> next;
  }
  return output;
}
/* Function to insert a node at the beginging of the linked list */

void push(__didxclass_Node *head_ref,int new_data)
{
/* allocate node */
  __didxclass_Node new_node = (__didxclass_Node )(__dst_alloc_malloc__dmemclass_Node(sizeof(struct Node )));
  if (new_node == 0L) {
    g_fallback = true;
    return ;
  }
/* put in the data */
  (&(__dmemclass_Node + new_node + 0U - 1U) -> _data) -> data = new_data;
/* link the old list off the new node */
  (&(__dmemclass_Node + new_node + 0U - 1U) -> _data) -> next =  *head_ref;
/* move the head to point to the new node */
   *head_ref = new_node;
}
extern "C" {
/* Drier program to test above functions*/

void process_top(int n,int *input,int *output,int *fallback)
{
  
#pragma HLS INTERFACE m_axi port = input offset = slave bundle = gmem
  
#pragma HLS INTERFACE m_axi port = output offset = slave bundle = gmem
  
#pragma HLS INTERFACE m_axi port = fallback offset = slave bundle = gmem
  
#pragma HLS INTERFACE s_axilite port = n bundle = control
  
#pragma HLS INTERFACE s_axilite port = input bundle = control
  
#pragma HLS INTERFACE s_axilite port = output bundle = control
  
#pragma HLS INTERFACE s_axilite port = fallback bundle = control
  
#pragma HLS INTERFACE s_axilite port = return bundle = control
/* Start with the empty list */
  __didxclass_Node res = 0L;
  __didxclass_Node a = 0L;
/* Let us create a unsorted linked lists to test the functions */
  for (int i = 0; i < n; i++) {
    push(&a,input[i]);
    if (g_fallback) 
      goto fail;
  }
/* Sort the above created Linked List */
  a = MergeSort(a);
  if (g_fallback) 
    goto fail;
  output = printList(a,output);
  fail:
   *fallback = g_fallback;
}
}
