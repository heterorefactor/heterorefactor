#include <stdio.h>
#include <stdlib.h>
#include "inputdata.h"
#include <time.h>
#include "bit.h"

void testbed(
        const float input[],
        const int   input_size,
        float       output[],
        int        *output_size);

int main()
{
	printf("Reading the samples\n");
	FILE* f = fopen("4224samples.txt", "r");

	float m;
	for (int i = 0; i < size; i++)
	{
			fscanf(f, "%f\n", &m);
			input[i] = m;
	}
	fclose(f);


    int output_size = 0;
    char filename[100];
    sprintf(filename,"%d",F+9);

    FILE* fw = fopen(filename, "w");
    testbed(input, size, output, &output_size);
    for (int i = 0; i < output_size / 3; i++)
        fprintf(fw, "%.6f\n", output[i]);

    for (int i = 0; i < output_size / 3; i++)
        fprintf(fw, "%.6f\n", output[i + output_size / 3]);

    for (int i = 0; i < output_size / 3; i++)
        fprintf(fw, "%.6f\n", output[i + output_size / 3 * 2]);

	return 0;
}

