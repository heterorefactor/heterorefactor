#include <ap_int.h>

extern "C" {
void rgb2yuv(
        ap_int<512> *r, ap_int<512> *g, ap_int<512> *b,
        ap_int<512> *y, ap_int<512> *u, ap_int<512> *v,
        int n);
};

void testbed(
        const float input[],
        const int   input_size,
        float       output[],
        int        *output_size) {

    int effective_input = input_size / 3 / 16 * 16;
    int effective_output = effective_input;
    int coalesced = effective_input / 16;

    int effective_size = effective_input * 3;
    int coalesced_size = effective_size / 16;
    *output_size = effective_size;

    ap_int<512> input_coalesced[coalesced_size];
    ap_int<512> output_coalesced[coalesced_size];

    for (int i = 0; i < coalesced_size; i++) {
        for (int j = 0; j < 16; j++) {
            int cast_input = input[i * 16 + j];
            input_coalesced[i](32 * (j+1) - 1, 32 * j) = cast_input;
        }
    }

    rgb2yuv(input_coalesced,
            input_coalesced + coalesced,
            input_coalesced + coalesced * 2,
            output_coalesced,
            output_coalesced + coalesced,
            output_coalesced + coalesced * 2,
            effective_input);

    for (int i = 0; i < coalesced_size; i++) {
        for (int j = 0; j < 16; j++) {
            int cast_output = output_coalesced[i](32 * (j+1) - 1, 32 * j);
            output[i * 16 + j] = *((float *)&cast_output);
        }
    }
}
