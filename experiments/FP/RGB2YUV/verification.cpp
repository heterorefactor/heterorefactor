#include "verification.h"

int verification(int F, int N, double th, double alpha, float ar[])
{
    float apr[N];

    int bit = F + 8 + 1;
    char filename[100];
    sprintf(filename,"./float_test/solution1/csim/build/%d", bit);

    float m = 0;
   	printf("Reading the approximate results\n");
	FILE* f2 = fopen(filename, "r");
	for (int h = 0; h < N; h++)
	{

		fscanf(f2, "%f ", &m);
		apr[h] = m;
	}
	fclose(f2);

	int pass = 0;
	double error[N];
	for (int h = 0; h < N; h++)
	{
		error[h] = fabs(ar[h]-apr[h]);
		if(error[h] < th)
		{
			pass++;
		}
//		error[h] = sqrt(tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2]);
	}
	printf("passed: %d\n", pass);

	if(pass < N*alpha)
	{
		printf("Verification failed at bit %d.\n", bit);
		printf("Verified bitwidth is: %d.\n", bit+1 );
		return 0;
	}
	printf("Verification succeed at bit %d.\n", bit);


   return 1;
}
