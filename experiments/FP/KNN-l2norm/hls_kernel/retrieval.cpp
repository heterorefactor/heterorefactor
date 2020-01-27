///*
// * retrieval.cpp
// *
// *  Created on: Jul 5, 2019
// *      Author: qzhang
// */
//
//
//#include "retrieval.h"
//
//const int E=8;
//const int F=22;
//
//typedef thls::policy_flopoco<E,F> fp_policy_t;
//typedef fp_policy_t::value_t fp_t;
//
//float l2norm(float query[3], float data[3], int dim)
//{
//	float dist = 0.0;
////	float check = 0.0;
//	fp_t fp_dist = thls::to<fp_t, thls::convert_policy(0xF)> (dist);
//
//	for (int j = 0; j < dim; j++)
//	{
//		fp_t fp_query = thls::to<fp_t, thls::convert_policy(0xF)> (query[j]);
//
//		fp_t fp_data = thls::to<fp_t, thls::convert_policy(0xF)> (data[j]);
//
//		fp_t neg = thls::to<fp_t, thls::convert_policy(0xF)>(-1.0f)*fp_data;
////		printf("neg data %f\n",thls::to<float>(fp_data));
//
//		fp_t abs = fp_query + thls::to<fp_t, thls::convert_policy(0xF)>(-1.0f)*fp_data;
//		fp_dist += abs * abs;
//
////		check += ((query[j] - data[j])*(query[j] - data[j]));
//	}
//	dist = thls::to<float>(fp_dist);
////	printf("dist: %f %f\n", dist, check);
//	dist = sqrt(dist);
//	return dist;
//}
//
//
//float l1norm(float query[3], float data[3], int dim)
//{
//	float dist = 0.0;
//	float check = 0.0;
//	fp_t fp_dist = thls::to<fp_t, thls::convert_policy(0xF)> (dist);
//
//	for (int j = 0; j < dim; j++)
//	{
//		fp_t fp_query = thls::to<fp_t, thls::convert_policy(0xF)> (query[j]);
//
//		fp_t fp_data = thls::to<fp_t, thls::convert_policy(0xF)> (data[j]);
//
//		fp_t fp_neg = thls::to<fp_t, thls::convert_policy(0xF)>(-1.0f)*fp_data;
//
//		fp_t a = fp_query + fp_neg;
//
//		if(thls::to<float>(a)<0)
//		{
//			a = thls::to<fp_t, thls::convert_policy(0xF)>(-1.0f)*a;
//		}
//
//		fp_dist += a;
//		check += fabs(query[j] - data[j]);
//	}
//	dist = thls::to<float>(fp_dist);
//	printf ("%f, %f\n", dist, check);
//	return dist;
//}
//
//
//
//float l0norm(float query[3], float data[3], int dim)
//{
//	float dist = 0.0;
//	float check = 0.0;
//	fp_t fp_dist = thls::to<fp_t, thls::convert_policy(0xF)> (dist);
//
//	for (int j = 0; j < dim; j++)
//	{
//		fp_t fp_query = thls::to<fp_t, thls::convert_policy(0xF)> (query[j]);
//		fp_t fp_data = thls::to<fp_t, thls::convert_policy(0xF)> (data[j]);
//		fp_t fp_neg = thls::to<fp_t, thls::convert_policy(0xF)>(-1.0f)*fp_data;
//		fp_t a = fp_query + fp_neg;
//
//		if(thls::to<float>(a) < 0)
//		{
//			a = thls::to<fp_t, thls::convert_policy(0xF)>(-1.0f)*a;
//		}
//
//		if(thls::to<float>(a) < eps)
//		{
//			fp_dist += thls::to<fp_t, thls::convert_policy(0xF)>(1.0f);
//		}
//		if(fabs(query[j]-data[j]) < eps)
//		{
//			check += 1;
////			printf("%f\n", check);
//		}
//	}
//	dist = thls::to<float>(fp_dist);
//	printf("%f %f\n", dist, check);
//	return check;
//}
//


#include <cstddef>
#include "thls/tops/policy_flopoco.hpp"
typedef thls::policy_flopoco<8, 15> __fpt_policy_t;
typedef thls::policy_flopoco<8,15> ::value_t __fpt_t;
#include <math.h>
#include <hls_stream.h>

#define dim (4096)

void data_read(ap_int<512> *query, ap_int<512> *data,
        hls::stream<float> query_s[16], hls::stream<float> data_s[16]) {
    for (int i = 0; i < dim/16; i++) {
#pragma HLS pipeline II=1
        ap_int<512> q = query[i], d = data[i];
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            int qq = q(32 * (j+1) - 1, 32 * j),
                dd = d(32 * (j+1) - 1, 32 * j);
			query_s[j].write(*(float *)&qq);
			data_s[j].write(*(float *)&dd);
        }
    }
}

void compute_diff(hls::stream<float> query_s[16], hls::stream<float> data_s[16],
        hls::stream<float> diff_s[16]) {
    for (int i = 0; i < dim/16; i++) {
#pragma HLS pipeline II=1
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            __fpt_t delta = ((__fpt_t )query_s[j].read()) -
                ((__fpt_t )data_s[j].read());
            __fpt_t result = delta * delta;
            diff_s[j].write(result);
        }
    }
}

__fpt_t compute_sum(hls::stream<float> diff_s[16]) {
    static __fpt_t sum[256];
#pragma HLS array_partition variable=sum dim=0 complete

    for (int i = 0; i < dim/16; i++) {
#pragma HLS pipeline II=1

        __fpt_t dist[16];
#pragma HLS array_partition variable=dist dim=0 complete
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            dist[j] = diff_s[j].read();
        }

        // expression is balanced by default
        sum[i] = dist[0] + dist[1] + dist[2] + dist[3] +
            dist[4] + dist[5] + dist[6] + dist[7] +
            dist[8] + dist[9] + dist[10] + dist[11] +
            dist[12] + dist[13] + dist[14] + dist[15];
    }

    static __fpt_t sum2[64];
#pragma HLS array_partition variable=sum2 dim=0 complete
    for (int i = 0; i < 64; i++) {
#pragma HLS unroll
        sum2[i] = sum[i * 4] + sum[i * 4 + 1] + sum[i * 4 + 2] + sum[i * 4 + 3];
    }

    static __fpt_t sum3[16];
#pragma HLS array_partition variable=sum3 dim=0 complete
    for (int i = 0; i < 16; i++) {
#pragma HLS unroll
        sum3[i] = sum2[i * 4] + sum2[i * 4 + 1] + sum2[i * 4 + 2] + sum2[i * 4 + 3];
    }

    static __fpt_t sum4[4];
#pragma HLS array_partition variable=sum3 dim=0 complete
    for (int i = 0; i < 4; i++) {
#pragma HLS unroll
        sum4[i] = sum3[i * 4] + sum3[i * 4 + 1] + sum3[i * 4 + 2] + sum3[i * 4 + 3];
    }

    return sqrt(sum4[0] + sum4[1] + sum4[2] + sum4[3]);
}

extern "C" {

float l2norm(ap_int<512> *query, ap_int<512> *data) // dim removed
{
#pragma HLS INTERFACE m_axi port=query offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=query bundle=control1
#pragma HLS INTERFACE s_axilite port=data bundle=control2
#pragma HLS INTERFACE s_axilite port=return bundle=control3

#pragma HLS dataflow

    hls::stream<float> query_s[16];
    hls::stream<float> data_s[16];
    hls::stream<float> diff_s[16];

    data_read(query, data, query_s, data_s);
    compute_diff(query_s, data_s, diff_s);
    return compute_sum(diff_s);
}

}
