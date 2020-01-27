#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "verification.h"

using namespace std;

int main(int argc , char  * argv[])
{
	double alpha = atof(argv[1]);
	double epsilon = atof(argv[2]);
	double th = atof(argv[3]);
        int N = 4223;

    //get the accurate result
        float ar[N];
 	float m;
    printf("Reading the accurate results\n");
	FILE* f1 = fopen("32", "r");
	for (int h = 0; h < N; h++)
	{

		fscanf(f1, "%f ", &m);
		ar[h] = m;
//		printf("%f\n", ar[h]);
	}
	fclose(f1);

	int F = 22;
	int flag = 1;
	int bit = 0;

	for(F=22; F>0; F--)
	{
        flag = verification(F, N, th, 0.999, ar);

        if (flag == 0){
            break;
        }
	}

	bit = F + 1;
	printf("The verified fraction bit is: %d.\n", bit);

    return 0;
}
