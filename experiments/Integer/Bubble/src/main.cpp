#include "bubble.h"
#include <stdio.h>
#include <stdlib.h>

//using namespace std;

int main()
{
	int c, d, swap, retval=0;
	int n;
    FILE* f = fopen("0.dat", "r");

    	int numbers[K]; // assuming there are only 5 numbers in the file
    	int i=0;

    	for (i=0; i<K; i++)
    	{
    		fscanf(f, "%d\n", &n);
    		numbers[i]=n;
//    		printf("%d, %d\n",i, numbers[i]);
    	}
//    	while( fscanf(f, "%d\n", &n) > 0 && i<K ) // parse %d followed by ','
//    	{
//        	numbers[i] = n;
//		printf("%d, %d\n",i, numbers[i]);
//        	i++;
//    	}

    	fclose(f);

	int array[K];
	for (c = 0; c < K; c++)
	{
		array[c] = numbers[c];
	}
	printf("Begin sort!\n");
    bubbleSort(array);

    for (int cnt=0; cnt<K; cnt++)
    {
    	printf("%d %d\n", cnt, array[cnt]);
    }

    return retval;

}
