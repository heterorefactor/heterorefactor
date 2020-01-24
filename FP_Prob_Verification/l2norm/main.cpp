#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void testbed(
        const float input[],
        const int   input_size,
        float       output[],
        int        *output_size);

int main(void) {
    int size = 16 * 2;

    float input[size];
	for (int i = 0; i < size; i++)
        input[i] = rand() % 256;

    float output[size];
    int output_size = 0;

    testbed(input, size, output, &output_size);
    printf("Output from kernel = %f\n", output[0]);

    float ans = 0;
    for (int i = 0; i < size / 2; i++) {
        float delta = input[i] - input[i + size / 2];
        ans += delta * delta;
    }
    printf("Expected output = %f\n", sqrt(ans));

	return 0;
}

