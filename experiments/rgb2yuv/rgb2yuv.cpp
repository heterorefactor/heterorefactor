#include <cstddef>
#include "thls/tops/policy_flopoco.hpp"
typedef thls::policy_flopoco<5,17> __fpt_policy_t;
typedef __fpt_policy_t::value_t __fpt_t;
// TODO: should be automatically prepended

#include <ap_int.h>
#include <hls_stream.h>

extern "C" {

void rgb2yuv(
        ap_int<512> *r, ap_int<512> *g, ap_int<512> *b,
        ap_int<512> *y, ap_int<512> *u, ap_int<512> *v,
        int n)
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

    hls::stream<int> input_r[16];
    hls::stream<int> input_g[16];
    hls::stream<int> input_b[16];
    hls::stream<float> output_y[16];
    hls::stream<float> output_u[16];
    hls::stream<float> output_v[16];

    for (int i = 0; i < n/16; i++) {
#pragma HLS pipeline II=1
        ap_int<512> rr = r[i], gg = g[i], bb = b[i];
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            input_r[j].write(rr(32 * (j+1) - 1, 32 * j));
            input_g[j].write(gg(32 * (j+1) - 1, 32 * j));
            input_b[j].write(bb(32 * (j+1) - 1, 32 * j));
        }
    }

    for (int i = 0; i < n/16; i++) {
#pragma HLS pipeline II=1
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            float r = input_r[j].read(), g = input_g[j].read(), b = input_b[j].read();
            output_y[j].write((0.299*r + 0.587*g + 0.114*b)/255.0);
            output_u[j].write((-0.147*r - 0.289*g + 0.436*b)/255.0);
            output_v[j].write((0.615*r - 0.515*g - 0.100*b)/255.0);
        }
    }

    for (int i = 0; i < n/16; i++) {
#pragma HLS pipeline II=1
        ap_int<512> yy, uu, vv;
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            float y = output_y[j].read();
            float u = output_u[j].read();
            float v = output_v[j].read();
            yy(32 * (j+1) - 1, 32 * j) = *((int *)(&y));
            uu(32 * (j+1) - 1, 32 * j) = *((int *)(&u));
            vv(32 * (j+1) - 1, 32 * j) = *((int *)(&v));
        }
        y[i] = yy; u[i] = uu; v[i] = vv;
    }
}

};

