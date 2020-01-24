#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inputdata.h"
#include "bit.h"

void testbed(
        const float input[],
        const int   input_size,
        float       output[],
        int        *output_size);

int main(void) {

	printf("Reading the samples\n");
	FILE* f = fopen("2944samples.txt", "r");

	int output_size = 0;
	char filename[100];
	sprintf(filename,"%d",F+9);
	FILE* fw = fopen(filename, "w");

	float m;
	for (int i = 0; i < size; i++)
	{
		fscanf(f, "%f\n", &m);
		int j = i%32;
		input[j] = m;

		if(j == 31)
		{
			testbed(input, 32, output, &output_size);
			fprintf(fw, "%f\n", output[0]);
		}
	}
	fclose(fw);
	fclose(f);


	return 0;
}
