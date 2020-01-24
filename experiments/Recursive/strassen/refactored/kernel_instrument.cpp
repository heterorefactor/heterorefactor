
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
const char *__dst_filename = "/tmp/hetero-profile";
unsigned long long __dst_file = 0;

struct __dst_alloc_list__dmemi 
{
  struct __dst_alloc_list_base_t _link;
  int _data;
}
;
struct __dst_alloc_list__dmemi __dmemi[1024U];
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
    return allocated - __dmemi + 1;
}
void __dst_alloc_free__dmemi(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
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

struct __dst_alloc_list__dmemUL 
{
  struct __dst_alloc_list_base_t _link;
  unsigned long long _data;
}
;
struct __dst_alloc_list__dmemUL __dmemUL[1024U];
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
__dst_alloc_size_t __dst_alloc_malloc__dmemUL(__dst_alloc_size_t request) {
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    request = request / sizeof(unsigned long long);
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] malloc " "__dmemUL"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    __dst_alloc_list__dmemUL *allocated = (__dst_alloc_list__dmemUL *)malloc(
            request * sizeof(__dst_alloc_list__dmemUL));
    allocated[0]._link.prev = request;
    return allocated - __dmemUL + 1;
}
void __dst_alloc_free__dmemUL(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    ptr -= 1;
    if (!__dst_file) {
        __dst_file = (unsigned long long)fopen(__dst_filename, "w");
    }
    __dst_alloc_size_t request = __dmemUL[ptr]._link.prev;
    fprintf((FILE *)__dst_file,
            "[__DST_ALLOC] free " "__dmemUL"
            " %llu\n", request);
    fflush((FILE *)__dst_file);
    free(__dmemUL + ptr);
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
    (&(__dmemUL + matA + i - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * l)));
    if (!(&(__dmemUL + matA + i - 1U) -> _data)[0U]) {
      g_fallback = true;
      goto fail;
    }
    for (int j = 0; j < l; j++) 
      (&(__dmemi + (&(__dmemUL + matA + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = mat1[i * l + j];
  }
  matB = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * l)));
  if (!matB) {
    g_fallback = true;
    goto fail;
  }
  for (int i = 0; i < l; i++) {
    (&(__dmemUL + matB + i - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * m)));
    if (!(&(__dmemUL + matB + i - 1U) -> _data)[0U]) {
      g_fallback = true;
      goto fail;
    }
    for (int j = 0; j < m; j++) 
      (&(__dmemi + (&(__dmemUL + matB + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = mat2[i * m + j];
  }
  matC = Strassen(matA,matB,n,l,m);
  if (g_fallback) 
    goto fail;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      mat3[i * m + j] = (&(__dmemi + (&(__dmemUL + matC + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U];
    }
    __dst_alloc_free__dmemi((&(__dmemUL + matC + i - 1U) -> _data)[0U]);
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
    (&(__dmemUL + c + i - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * m)));
    if (!(&(__dmemUL + c + i - 1U) -> _data)[0U]) {
      g_fallback = true;
      return 0L;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      (&(__dmemi + (&(__dmemUL + c + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = 0;
      for (int k = 0; k < l; k++) {
        (&(__dmemi + (&(__dmemUL + c + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] += (&(__dmemi + (&(__dmemUL + a + i - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] * (&(__dmemi + (&(__dmemUL + b + k - 1U) -> _data)[0U] + j - 1U) -> _data)[0U];
      }
    }
  }
  return c;
}

__didx__Pb__i__Pe__ Strassen(__didx__Pb__i__Pe__ a,__didx__Pb__i__Pe__ b,int n,int l,int m)
{
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] call L5457R__L5458R\n");fflush((FILE *)__dst_file);
  if (n == 1 || l == 1 || m == 1) {
    __didx__Pb__i__Pe__ __temp0__ = (__didx__Pb__i__Pe__ )(MatrixMultiply(a,b,n,l,m));
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp0__;
  }
  __didx__Pb__i__Pe__ c = (__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * n));
  if (!c) {
    g_fallback = true;
    __didx__Pb__i__Pe__ __temp1__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp1__;
  }
  for (int i = 0; i < n; i++) {
    (&(__dmemUL + c + i - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * m)));
    if (!(&(__dmemUL + c + i - 1U) -> _data)[0U]) {
      g_fallback = true;
      __didx__Pb__i__Pe__ __temp2__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
      0;
      return __temp2__;
    }
  }
  int adjN = (n >> 1) + (n & 1);
  int adjL = (l >> 1) + (l & 1);
  int adjM = (m >> 1) + (m & 1);
  __didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ As = (__didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int ***) * 2));
  if (!As) {
    g_fallback = true;
    __didx__Pb__i__Pe__ __temp3__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp3__;
  }
  for (int x = 0; x < 2; x++) {
    (&(__dmemUL + As + x - 1U) -> _data)[0U] = ((__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 2)));
    if (!(&(__dmemUL + As + x - 1U) -> _data)[0U]) {
      g_fallback = true;
      __didx__Pb__i__Pe__ __temp4__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
      0;
      return __temp4__;
    }
    for (int y = 0; y < 2; y++) {
      (&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp5__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp5__;
      }
      for (int i = 0; i < adjN; i++) {
        (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjL)));
        if (!(&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp6__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp6__;
        }
        for (int j = 0; j < adjL; j++) {
          int I = i + (x & 1) * adjN;
          int J = j + (y & 1) * adjL;
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = (I < n && J < l?(&(__dmemi + (&(__dmemUL + a + I - 1U) -> _data)[0U] + J - 1U) -> _data)[0U] : 0);
        }
      }
    }
  }
  __didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ Bs = (__didx__Pb____Pb____Pb__i__Pe____Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int ***) * 2));
  if (!Bs) {
    g_fallback = true;
    __didx__Pb__i__Pe__ __temp7__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp7__;
  }
  for (int x = 0; x < 2; x++) {
    (&(__dmemUL + Bs + x - 1U) -> _data)[0U] = ((__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 2)));
    if (!(&(__dmemUL + Bs + x - 1U) -> _data)[0U]) {
      g_fallback = true;
      __didx__Pb__i__Pe__ __temp8__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
      0;
      return __temp8__;
    }
    for (int y = 0; y < 2; y++) {
      (&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp9__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp9__;
      }
      for (int i = 0; i < adjL; i++) {
        (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjM)));
        if (!(&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp10__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp10__;
        }
        for (int j = 0; j < adjM; j++) {
          int I = i + (x & 1) * adjL;
          int J = j + (y & 1) * adjM;
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = (I < l && J < m?(&(__dmemi + (&(__dmemUL + b + I - 1U) -> _data)[0U] + J - 1U) -> _data)[0U] : 0);
        }
      }
    }
  }
  __didx__Pb____Pb__i__Pe____Pe__ s = (__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 10));
  if (!s) {
    g_fallback = true;
    __didx__Pb__i__Pe__ __temp11__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp11__;
  }
  for (int i = 0; i < 10; i++) {
    switch(i){
      case 0:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjL)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp12__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp12__;
      }
      for (int j = 0; j < adjL; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjM)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp13__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp13__;
        }
        for (int k = 0; k < adjM; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 0 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 1:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp14__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp14__;
      }
      for (int j = 0; j < adjN; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjL)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp15__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp15__;
        }
        for (int k = 0; k < adjL; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 0 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 2:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp16__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp16__;
      }
      for (int j = 0; j < adjN; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjL)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp17__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp17__;
        }
        for (int k = 0; k < adjL; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 1 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 3:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjL)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp18__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp18__;
      }
      for (int j = 0; j < adjL; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjM)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp19__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp19__;
        }
        for (int k = 0; k < adjM; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 1 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 4:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp20__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp20__;
      }
      for (int j = 0; j < adjN; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjL)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp21__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp21__;
        }
        for (int k = 0; k < adjL; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 5:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjL)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp22__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp22__;
      }
      for (int j = 0; j < adjL; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjM)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp23__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp23__;
        }
        for (int k = 0; k < adjM; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 6:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp24__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp24__;
      }
      for (int j = 0; j < adjN; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjL)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp25__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp25__;
        }
        for (int k = 0; k < adjL; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 0 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 7:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjL)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp26__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp26__;
      }
      for (int j = 0; j < adjL; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjM)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp27__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp27__;
        }
        for (int k = 0; k < adjM; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 1 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 8:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjN)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp28__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp28__;
      }
      for (int j = 0; j < adjN; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjL)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp29__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp29__;
        }
        for (int k = 0; k < adjL; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + 1 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
      case 9:
      (&(__dmemUL + s + i - 1U) -> _data)[0U] = ((__didx__Pb__i__Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int *) * adjL)));
      if (!(&(__dmemUL + s + i - 1U) -> _data)[0U]) {
        g_fallback = true;
        __didx__Pb__i__Pe__ __temp30__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
        0;
        return __temp30__;
      }
      for (int j = 0; j < adjL; j++) {
        (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * adjM)));
        if (!(&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]) {
          g_fallback = true;
          __didx__Pb__i__Pe__ __temp31__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
          0;
          return __temp31__;
        }
        for (int k = 0; k < adjM; k++) {
          (&(__dmemi + (&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + 0 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + k - 1U) -> _data)[0U];
        }
      }
      break; 
    }
  }
  __didx__Pb____Pb__i__Pe____Pe__ p = (__didx__Pb____Pb__i__Pe____Pe__ )(__dst_alloc_malloc__dmemUL(sizeof(int **) * 7));
  if (!p) {
    g_fallback = true;
    __didx__Pb__i__Pe__ __temp32__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp32__;
  }
  (&(__dmemUL + p + 0 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + (&(__dmemUL + As + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U],(&(__dmemUL + s + 0 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp33__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp33__;
  }
  (&(__dmemUL + p + 1 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + s + 1 - 1U) -> _data)[0U],(&(__dmemUL + (&(__dmemUL + Bs + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp34__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp34__;
  }
  (&(__dmemUL + p + 2 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + s + 2 - 1U) -> _data)[0U],(&(__dmemUL + (&(__dmemUL + Bs + 0 - 1U) -> _data)[0U] + 0 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp35__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp35__;
  }
  (&(__dmemUL + p + 3 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + (&(__dmemUL + As + 1 - 1U) -> _data)[0U] + 1 - 1U) -> _data)[0U],(&(__dmemUL + s + 3 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp36__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp36__;
  }
  (&(__dmemUL + p + 4 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + s + 4 - 1U) -> _data)[0U],(&(__dmemUL + s + 5 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp37__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp37__;
  }
  (&(__dmemUL + p + 5 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + s + 6 - 1U) -> _data)[0U],(&(__dmemUL + s + 7 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp38__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp38__;
  }
  (&(__dmemUL + p + 6 - 1U) -> _data)[0U] = Strassen((&(__dmemUL + s + 8 - 1U) -> _data)[0U],(&(__dmemUL + s + 9 - 1U) -> _data)[0U],adjN,adjL,adjM);
  if (g_fallback) {
    __didx__Pb__i__Pe__ __temp39__ = (__didx__Pb__i__Pe__ )((int **)0L);
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
    0;
    return __temp39__;
  }
  for (int i = 0; i < adjN; i++) {
    for (int j = 0; j < adjM; j++) {
      (&(__dmemi + (&(__dmemUL + c + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 4 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 3 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 1 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 5 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U];
      if (j + adjM < m) 
        (&(__dmemi + (&(__dmemUL + c + i - 1U) -> _data)[0U] + (j + adjM) - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 0 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 1 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U];
      if (i + adjN < n) 
        (&(__dmemi + (&(__dmemUL + c + (i + adjN) - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 2 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 3 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U];
      if (i + adjN < n && j + adjM < m) 
        (&(__dmemi + (&(__dmemUL + c + (i + adjN) - 1U) -> _data)[0U] + (j + adjM) - 1U) -> _data)[0U] = (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 4 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] + (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 0 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 2 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U] - (&(__dmemi + (&(__dmemUL + (&(__dmemUL + p + 6 - 1U) -> _data)[0U] + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U];
    }
  }
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int i = 0; i < adjN; i++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U]);
      }
      __dst_alloc_free__dmemUL((&(__dmemUL + (&(__dmemUL + As + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U]);
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + As + x - 1U) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(As);
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int i = 0; i < adjL; i++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U] + i - 1U) -> _data)[0U]);
      }
      __dst_alloc_free__dmemUL((&(__dmemUL + (&(__dmemUL + Bs + x - 1U) -> _data)[0U] + y - 1U) -> _data)[0U]);
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + Bs + x - 1U) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(Bs);
  for (int i = 0; i < 10; i++) {
    switch(i){
      case 0:;
      case 3:;
      case 5:;
      case 7:;
      case 9:
      for (int j = 0; j < adjL; j++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]);
      }
      break; 
      case 1:;
      case 2:;
      case 4:;
      case 6:;
      case 8:
      for (int j = 0; j < adjN; j++) {
        __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + s + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]);
      }
      break; 
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + s + i - 1U) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(s);
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < n >> 1; j++) {
      __dst_alloc_free__dmemi((&(__dmemUL + (&(__dmemUL + p + i - 1U) -> _data)[0U] + j - 1U) -> _data)[0U]);
    }
    __dst_alloc_free__dmemUL((&(__dmemUL + p + i - 1U) -> _data)[0U]);
  }
  __dst_alloc_free__dmemUL(p);
  __didx__Pb__i__Pe__ __temp40__ = (__didx__Pb__i__Pe__ )c;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
  0;
  return __temp40__;
if (!__dst_file) { __dst_file = (unsigned long long)fopen(__dst_filename, "w"); }fprintf((FILE *)__dst_file, "[__REC_RECUR] ret L5457R__L5458R\n");fflush((FILE *)__dst_file);
}
