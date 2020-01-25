#include "bubble.h"
#include <stdio.h>
#include <stdlib.h>

//using namespace std;

bool guard_error_main = false;
void guard_main(int value,int size){
 #pragma HLS inline off 
 if(value>(1<<size)){guard_error_main= true;}
}
int main()
{
	int c, d, swap, retval=0;
	int n;
    FILE* f = fopen("0.dat", "r");

    	ap_uint<4> numbers[K]; // assuming there are only 5 numbers in the file
    	int i=0;

    	for (i=0; i<K; i++)
    	{
    		fscanf(f, "%d\n", &n);
    		numbers[i]=n;
guard_main(n,4);
//    		printf("%d, %d\n",i, numbers[i]);
    	}
//    	while( fscanf(f, "%d\n", &n) > 0 && i<K ) // parse %d followed by ','
//    	{
//        	numbers[i] = n;
//		printf("%d, %d\n",i, numbers[i]);
//        	i++;
//    	}

    	fclose(f);

	ap_uint<4> array[K];
	for (c = 0; c < K; c++)
	{
		array[c] = numbers[c];
guard_main(numbers[c],4);
	}
	printf("Begin sort!\n");
    bubbleSort(array);

    for (int cnt=0; cnt<K; cnt++)
    {
    	printf("%d %d\n", cnt, array[cnt]);
    }

    return retval;

}
