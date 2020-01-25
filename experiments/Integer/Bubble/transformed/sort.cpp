#include "bubble.h"
#include "stdio.h"


bool guard_error_sort = false;
void guard_sort(int value,int size){
 #pragma HLS inline off 
 if(value>(1<<size)){guard_error_sort= true;}
}
void swap(ap_uint<4> arr[], ap_uint<9> i, ap_uint<9> j)
{
	ap_uint<4> temp;
	if(i == j)
	{
		return;
	}
	temp = arr[i];
guard_sort(arr[i],4); // swap elements
	arr[i] = arr[j];
guard_sort(arr[j],4);
	arr[j] = temp;
guard_sort(temp,4);

}

void bubbleSort(ap_uint<4> num[K])
{
	ap_uint<9> i, j, q;
	ap_uint<4> num_l[K];

//	if(K==0)
//	{
//		return;
//	}

	for(q=0;q< K;q++)
	{
		num_l[q] =  num[q];
guard_sort(num[q],4);
	}

  	for(i = 0; i < K -1 ; i++)
	{
    		for(j = 0; j < K -1 -i ; j++)
		{
			    //printf("%d\n",num_l[j]);
      			if(num_l[j] > num_l[j+1])
      			{
				swap(num_l, j, j+1);
			}
    		}
  	}

	for(q =0; q< K; q++)
	{
        	num[q] =  num_l[q];
guard_sort(num_l[q],4);
	}
	return; //arrays are passed to functions by address; nothing is returned
}