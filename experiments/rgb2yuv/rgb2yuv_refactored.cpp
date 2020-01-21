// === BEGIN FP SUPPORT LIBRARY ===
#include <cstddef>
#include "thls/tops/policy_flopoco.hpp"
typedef thls::policy_flopoco< 5 , 17 > __fpt_policy_t;
typedef thls::policy_flopoco< 5,17> ::value_t __fpt_t;
// === END FP SUPPORT LIBRARY ===
#include <ap_int.h>
#include <hls_stream.h>
extern "C" {

void rgb2yuv(class ap_int< 512 > *r,class ap_int< 512 > *g,class ap_int< 512 > *b,class ap_int< 512 > *y,class ap_int< 512 > *u,class ap_int< 512 > *v,int n)
{
  
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=y bundle=control
  
#pragma HLS INTERFACE m_axi port=u offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=u bundle=control
  
#pragma HLS INTERFACE m_axi port=v offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=v bundle=control
  
#pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=r bundle=control
  
#pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=b bundle=control
  
#pragma HLS INTERFACE m_axi port=g offset=slave bundle=gmem
  
#pragma HLS INTERFACE s_axilite port=g bundle=control
  
#pragma HLS INTERFACE s_axilite port=n bundle=control
  
#pragma HLS INTERFACE s_axilite port=return bundle=control
  
#pragma HLS dataflow
  hls::stream< int  > input_r[16];
  hls::stream< int  > input_g[16];
  hls::stream< int  > input_b[16];
  hls::stream< float  > output_y[16];
  hls::stream< float  > output_u[16];
  hls::stream< float  > output_v[16];
  for (int i = 0; i < n / 16; i++) {
    
#pragma HLS pipeline II=1
    class ap_int< 512 > rr(r[i]);
    class ap_int< 512 > gg(g[i]);
    class ap_int< 512 > bb(b[i]);
    for (int j = 0; j < 16; j++) {
      
#pragma HLS unroll
      input_r[j] . write((rr((32 * (j + 1) - 1),(32 * j))));
      input_g[j] . write((gg((32 * (j + 1) - 1),(32 * j))));
      input_b[j] . write((bb((32 * (j + 1) - 1),(32 * j))));
    }
  }
  for (int i = 0; i < n / 16; i++) {
    
#pragma HLS pipeline II=1
    for (int j = 0; j < 16; j++) {
      
#pragma HLS unroll
      __fpt_t r = (input_r[j] . read());
      __fpt_t g = (input_g[j] . read());
      __fpt_t b = (input_b[j] . read());
      output_y[j] . write(((__fpt_t )(((__fpt_t )(((__fpt_t )(((__fpt_t )(((__fpt_t )0.299) * ((__fpt_t )r))) + ((__fpt_t )(((__fpt_t )0.587) * ((__fpt_t )g))))) + ((__fpt_t )(((__fpt_t )0.114) * ((__fpt_t )b))))) / ((__fpt_t )255.0))));
      output_u[j] . write(((__fpt_t )(((__fpt_t )(((__fpt_t )(((__fpt_t )(((__fpt_t )(- 0.147)) * ((__fpt_t )r))) - ((__fpt_t )(((__fpt_t )0.289) * ((__fpt_t )g))))) + ((__fpt_t )(((__fpt_t )0.436) * ((__fpt_t )b))))) / ((__fpt_t )255.0))));
      output_v[j] . write(((__fpt_t )(((__fpt_t )(((__fpt_t )(((__fpt_t )(((__fpt_t )0.615) * ((__fpt_t )r))) - ((__fpt_t )(((__fpt_t )0.515) * ((__fpt_t )g))))) - ((__fpt_t )(((__fpt_t )0.100) * ((__fpt_t )b))))) / ((__fpt_t )255.0))));
    }
  }
  for (int i = 0; i < n / 16; i++) {
    
#pragma HLS pipeline II=1
    class ap_int< 512 > yy;
    class ap_int< 512 > uu;
    class ap_int< 512 > vv;
    for (int j = 0; j < 16; j++) {
      
#pragma HLS unroll
      float y = output_y[j] . read(); // TODO: fix me
      float u = output_u[j] . read();
      float v = output_v[j] . read();
      yy((32 * (j + 1) - 1),(32 * j)) =  *((int *)(&y));
      uu((32 * (j + 1) - 1),(32 * j)) =  *((int *)(&u));
      vv((32 * (j + 1) - 1),(32 * j)) =  *((int *)(&v));
    }
    y[i] = yy;
    u[i] = uu;
    v[i] = vv;
  }
}
}
