#include <math.h>
#include <hls_stream.h>
#include <ap_int.h>

void data_read(ap_int<512> *query, ap_int<512> *data,
        hls::stream<float> query_s[16], hls::stream<float> data_s[16],
        int dim) {
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
        hls::stream<float> diff_s[16], int dim) {
    for (int i = 0; i < dim/16; i++) {
#pragma HLS pipeline II=1
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            float delta = query_s[j].read() -
                data_s[j].read();
            float result = delta * delta;
            diff_s[j].write(result);
        }
    }
}

float compute_sum(hls::stream<float> diff_s[16], int dim) {
    static float sum[256];
#pragma HLS array_partition variable=sum dim=0 complete

    for (int i = 0; i < dim/16; i++) {
#pragma HLS pipeline II=1

        float dist[16];
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

    static float sum2[64];
#pragma HLS array_partition variable=sum2 dim=0 complete
    for (int i = 0; i < 64; i++) {
#pragma HLS unroll
        sum2[i] = sum[i * 4] + sum[i * 4 + 1] + sum[i * 4 + 2] + sum[i * 4 + 3];
    }

    static float sum3[16];
#pragma HLS array_partition variable=sum3 dim=0 complete
    for (int i = 0; i < 16; i++) {
#pragma HLS unroll
        sum3[i] = sum2[i * 4] + sum2[i * 4 + 1] + sum2[i * 4 + 2] + sum2[i * 4 + 3];
    }

    static float sum4[4];
#pragma HLS array_partition variable=sum3 dim=0 complete
    for (int i = 0; i < 4; i++) {
#pragma HLS unroll
        sum4[i] = sum3[i * 4] + sum3[i * 4 + 1] + sum3[i * 4 + 2] + sum3[i * 4 + 3];
    }

    return sqrt(sum4[0] + sum4[1] + sum4[2] + sum4[3]);
}

extern "C" {

void l2norm(ap_int<512> *query, ap_int<512> *data, float *ret, int dim)
{
#pragma HLS INTERFACE m_axi port=query offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=ret offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=query bundle=control
#pragma HLS INTERFACE s_axilite port=data bundle=control
#pragma HLS INTERFACE s_axilite port=ret bundle=control
#pragma HLS INTERFACE s_axilite port=dim bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS dataflow

    hls::stream<float> query_s[16];
    hls::stream<float> data_s[16];
    hls::stream<float> diff_s[16];

    data_read(query, data, query_s, data_s, dim);
    compute_diff(query_s, data_s, diff_s, dim);
    *ret = compute_sum(diff_s, dim);
}

}
