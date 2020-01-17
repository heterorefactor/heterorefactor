#include <stdio.h>
#include <stdlib.h>

void testbed(
        const float input[],
        const int   input_size,
        float       output[],
        int        *output_size);

int main(void) {
    int size = 16 * 3;

    float input[size];
	for (int i = 0; i < size; i++)
        input[i] = rand() % 256;

    float output[size];
    int output_size = 0;

    testbed(input, size, output, &output_size);
    printf("Y: ");
    for (int i = 0; i < output_size / 3; i++)
        printf("% 7.3f ", output[i]);
    printf("\nU: ");
    for (int i = 0; i < output_size / 3; i++)
        printf("% 7.3f ", output[i + output_size / 3]);
    printf("\nV: ");
    for (int i = 0; i < output_size / 3; i++)
        printf("% 7.3f ", output[i + output_size / 3 * 2]);
    printf("\n");

	return 0;
}

