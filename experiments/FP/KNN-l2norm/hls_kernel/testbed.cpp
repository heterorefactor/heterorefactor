#include <ap_int.h>

extern "C" {
void l2norm(ap_int<512> *query, ap_int<512> *data, float *ret, int dim);
};

void testbed(
        const float input[],
        const int   input_size,
        float       output[],
        int        *output_size) {

    int effective_input = input_size / 2 / 16 * 16;
    int coalesced = effective_input / 16;

    int effective_size = effective_input * 2;
    int coalesced_size = effective_size / 16;
    *output_size = 1;

    ap_int<512> input_coalesced[coalesced_size];
    for (int i = 0; i < coalesced_size; i++) {
        for (int j = 0; j < 16; j++) {
            int cast_input = *((int *)&input[i * 16 + j]);
            input_coalesced[i](32 * (j+1) - 1, 32 * j) = cast_input;
        }
    }

    l2norm(input_coalesced,
            input_coalesced + coalesced,
            output,
            effective_input);
}
