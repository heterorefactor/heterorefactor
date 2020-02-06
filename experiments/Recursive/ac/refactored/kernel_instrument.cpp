
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxc was char *
typedef unsigned long long __didxc;
// type __didxclass_node was struct ::node{int substring_index;struct node *fail;struct node *next[26];}*
typedef unsigned long long __didxclass_node;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "/tmp/hetero-profile";
unsigned long long __dst_file = 0;

struct __dst_alloc_list__dmemc 
{
  struct __dst_alloc_list_base_t _link;
  char _data;
}
;
struct __dst_alloc_list__dmemc __dmemc[1025U];
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
__dst_alloc_size_t __dst_alloc_malloc__dmemc(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(char);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemc"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemc *allocated = (__dst_alloc_list__dmemc *)malloc(
            request * sizeof(__dst_alloc_list__dmemc));
    allocated[0]._link.prev = request;
    return allocated - __dmemc;
}
void __dst_alloc_free__dmemc(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemc[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemc"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemc + ptr);
}
unsigned long long __dst_alloc_malloc__dmemc(unsigned long long );
void __dst_alloc_free__dmemc(unsigned long long );
// === END REC SUPPORT LIBRARY ===
#include <cassert>
#include <cstdlib>
/*
 * Trie tree with Aho-Corasick state machine.
 *
 * de la Briandais, René (1959). File searching using variable length keys.
 * Proc. Western J. Computer Conf. pp. 295–298. Cited by Brass.
 *
 * Aho, Alfred V.; Corasick, Margaret J. (June 1975). "Efficient string
 * matching: An aid to bibliographic search". Communications of the ACM.
 * 18 (6): 333–340. doi:10.1145/360825.360855. MR 0371172.
 */

struct node 
{
  int substring_index;
  __didxclass_node fail;
  __didxclass_node next[26];
}
;

struct __dst_alloc_list__dmemclass_node 
{
  struct __dst_alloc_list_base_t _link;
  struct node _data;
}
;
struct __dst_alloc_list__dmemclass_node __dmemclass_node[1025U];
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
__dst_alloc_size_t __dst_alloc_malloc__dmemclass_node(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(struct ::node);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemclass_node"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemclass_node *allocated = (__dst_alloc_list__dmemclass_node *)malloc(
            request * sizeof(__dst_alloc_list__dmemclass_node));
    allocated[0]._link.prev = request;
    return allocated - __dmemclass_node;
}
void __dst_alloc_free__dmemclass_node(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemclass_node[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemclass_node"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemclass_node + ptr);
}
unsigned long long __dst_alloc_malloc__dmemclass_node(unsigned long long );
void __dst_alloc_free__dmemclass_node(unsigned long long );
int node_count = 1;
bool g_fallback = false;

__didxclass_node new_node()
{
  __didxclass_node curr = (__didxclass_node )(__dst_alloc_malloc__dmemclass_node(sizeof(struct node )));
  if (!curr) {
    g_fallback = true;
    return 0L;
  }
  (&(__dmemclass_node + curr + 0U) -> _data) -> substring_index = - 1;
  (&(__dmemclass_node + curr + 0U) -> _data) -> fail = 0L;
  for (int i = 0; i < 26; i++) {
    (&(__dmemclass_node + curr + 0U) -> _data) -> next[i] = 0L;
  }
  return curr;
}
/*
 * Insert a new trie node with string as content.
 * The node will be inserted to the trie specified by root.
 *
 * Return: the '%' location of the string.
 */

int insert_node(__didxclass_node root,__didxc str,int substring_index)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5464R__L5465R\n");fflush((FILE *)__dst_file);
  char ch =  *(&(__dmemc + str + 0U) -> _data);
  if (ch == '%') {
    (&(__dmemclass_node + root + 0U) -> _data) -> substring_index = substring_index;
    int __temp0__ = (int )0;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5464R__L5465R\n");fflush((FILE *)__dst_file);
    0;
    return __temp0__;
  }
   else {
    ch >= 'a' && ch <= 'z'?(static_cast < void  >  (0)) : __assert_fail("ch >= 'a' && ch <= 'z'","hetero-689M3U-kernel.cpp",53,__PRETTY_FUNCTION__);
    int idx = ch - 'a';
    if (!(&(__dmemclass_node + root + 0U) -> _data) -> next[idx]) {
      (&(__dmemclass_node + root + 0U) -> _data) -> next[idx] = new_node();
      if (!(&(__dmemclass_node + root + 0U) -> _data) -> next[idx]) {
        int __temp1__ = (int )0;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5464R__L5465R\n");fflush((FILE *)__dst_file);
        0;
        return __temp1__;
      }
      node_count += 1;
    }
    int __temp2__ = (int )(insert_node((&(__dmemclass_node + root + 0U) -> _data) -> next[idx],str + 1,substring_index) + 1);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5464R__L5465R\n");fflush((FILE *)__dst_file);
    0;
    return __temp2__;
  }
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5464R__L5465R\n");fflush((FILE *)__dst_file);
}
/*
 * Build Aho-Corasick state machine.
 */

void build_AhoCorasick(__didxclass_node root,int node_count)
{
  __didxclass_node queue[16384];
// initialize queue
  int head = 0;
  int tail = 1;
  queue[0] = root;
  for (; head < tail; head++) {
    __didxclass_node curr = queue[head];
    for (int i = 0; i < 26; i++) {
// non-existent node
      if (!(&(__dmemclass_node + curr + 0U) -> _data) -> next[i]) 
        continue; 
// Aho-Corasick fail link
      (&(__dmemclass_node + (&(__dmemclass_node + curr + 0U) -> _data) -> next[i] + 0U) -> _data) -> fail = root;
      for (__didxclass_node p = (&(__dmemclass_node + curr + 0U) -> _data) -> fail; p; p = (&(__dmemclass_node + p + 0U) -> _data) -> fail) {
        if ((&(__dmemclass_node + p + 0U) -> _data) -> next[i]) {
          (&(__dmemclass_node + (&(__dmemclass_node + curr + 0U) -> _data) -> next[i] + 0U) -> _data) -> fail = (&(__dmemclass_node + p + 0U) -> _data) -> next[i];
          break; 
        }
      }
// add to queue
      queue[tail++] = (&(__dmemclass_node + curr + 0U) -> _data) -> next[i];
    }
  }
}
/*
 * Query on Aho-Corasick state machine and write matched indexes.
 */

void query_AhoCorasick(__didxclass_node root,char *query,int *substring_indexes,int *query_indexes)
{
  __didxclass_node curr = root;
  for (int offset = 0; ; offset++) {
    char ch = query[offset];
    if (ch == '%') 
      break; 
    ch >= 'a' && ch <= 'z'?(static_cast < void  >  (0)) : __assert_fail("ch >= 'a' && ch <= 'z'","hetero-689M3U-kernel.cpp",106,__PRETTY_FUNCTION__);
    int idx = ch - 'a';
// follow fail link if not matched in curr
    while(!(&(__dmemclass_node + curr + 0U) -> _data) -> next[idx] && curr != root)
      curr = (&(__dmemclass_node + curr + 0U) -> _data) -> fail;
// if matched next char
    if ((&(__dmemclass_node + curr + 0U) -> _data) -> next[idx]) 
      curr = (&(__dmemclass_node + curr + 0U) -> _data) -> next[idx];
// follow fail link to check matches
    for (__didxclass_node follow = curr; follow != root; follow = (&(__dmemclass_node + follow + 0U) -> _data) -> fail) {
      if ((&(__dmemclass_node + follow + 0U) -> _data) -> substring_index != - 1) {
         *(substring_indexes++) = (&(__dmemclass_node + follow + 0U) -> _data) -> substring_index;
         *(query_indexes++) = offset;
      }
    }
  }
// end indexes
   *substring_indexes = - 1;
   *query_indexes = - 1;
}
/*
 * Delete the whole tree
 */

void delete_tree(__didxclass_node root)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5470R__L5471R\n");fflush((FILE *)__dst_file);
  if (!root) {
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5470R__L5471R\n");fflush((FILE *)__dst_file);
    0;
    return ;
  }
  for (int i = 0; i < 26; i++) 
    delete_tree((&(__dmemclass_node + root + 0U) -> _data) -> next[i]);
  __dst_alloc_free__dmemclass_node(root);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5470R__L5471R\n");fflush((FILE *)__dst_file);
}
extern "C" {
/*
 * Implementation of Aho-Corasick Algorithm
 *
 * Search substrings in query, and fill matches in result arrays,
 * Assume all strings contain only letters 'a'-'z'.
 *
 * Parameter Format:
 *   substrings: multiple strings (target substrings) each with '%' ending.
 *               e.g. "string1%string2%", substring_length = 7+1+7+1.
 *   query: a string (the query document) with '%' ending
 *   substring_indexes: an output array, the indexes of found substrings.
 *   query_indexes: an output array, the corresponding indexes in query.
 */

void process_top(int *substring_length_p,char *substrings,char *query,int *substring_indexes,int *query_indexes,int *fallback)
{
  
#pragma HLS INTERFACE m_axi port=substring_length_p offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=substrings offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=query offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=substring_indexes offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=query_indexes offset=slave bundle=gmem
  
#pragma HLS INTERFACE m_axi port=fallback offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=substring_length_p bundle=control
  
#pragma HLS INTERFACE s_axilite port=substrings bundle=control
  
#pragma HLS INTERFACE s_axilite port=query bundle=control
  
#pragma HLS INTERFACE s_axilite port=substring_indexes bundle=control
  
#pragma HLS INTERFACE s_axilite port=query_indexes bundle=control
  
#pragma HLS INTERFACE s_axilite port=fallback bundle=control
  
#pragma HLS INTERFACE s_axilite port=return bundle=control
  int substring_length =  *substring_length_p;
  __didxclass_node root = new_node();
  __didxc substring_buf = (__didxc )(__dst_alloc_malloc__dmemc(sizeof(char ) * substring_length));
  if (!substring_buf) {
    g_fallback = true;
    goto fail;
  }
  for (int i = 0; i < substring_length; i++) {
    (&(__dmemc + substring_buf + i) -> _data)[0U] = substrings[i];
  }
  for (int offset = 0; offset < substring_length; ) {
    offset += insert_node(root,substring_buf + offset,offset) + 1;
    if (g_fallback) 
      goto fail;
  }
  build_AhoCorasick(root,node_count);
  query_AhoCorasick(root,query,substring_indexes,query_indexes);
  delete_tree(root);
  if (g_fallback) 
    goto fail;
  fail:
   *fallback = g_fallback;
}
}
