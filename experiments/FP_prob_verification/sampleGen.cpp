#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "sampleGen.h"

//sample generation for rgb2yuv
/*
int sample_gen(double alpha, double epsilon)
{
	int N;
	double result = log(2/(1-alpha))/(2*epsilon*epsilon);
    N = ceil(result/48)*48;
    printf("Required number of samples: %d\n", N);

    float input[N];

    char filename[100];
    sprintf(filename,"%dsamples.txt",N);

	FILE* fw = fopen(filename, "w");
	for (int i = 0; i < N; i++)
	{
            input[i] = rand() % 256;
            fprintf(fw, "%f\n", input[i]);
	}
	fclose(fw);

	printf("Sample %s generated!\n", filename);

	return N;
}
*/

//sample generation for knn-l2norm
int sample_gen(double alpha, double epsilon)
{
	int N;
	double result = log(2/(1-alpha))/(2*epsilon*epsilon);
    N = ceil(result);
    printf("Required number of samples: %d\n", N);

    char filename[100];
    sprintf(filename,"%dsamples.txt",N);

    //each 16*2 data represents one sample execution and we need N executions in total
    int NN = N * 32;
    float input[NN];

	FILE* fw = fopen(filename, "w");
	for (int i = 0; i < NN; i++)
	{
            input[i] = rand() % 256;
            fprintf(fw, "%f\n", input[i]);
	}
	fclose(fw);

	printf("Sample %s generated!\n", filename);

	return N;
}
