#include <iostream>
#include <fstream>
#include <stdlib.h>


#include "sampleGen.h"
#include "verification.h"

using namespace std;

string charToStr(char * contentChar)
{
	string tempStr;
	for (int i=0;contentChar[i]!='\0';i++)
	{
		tempStr+=contentChar[i];
	}
	return tempStr;
}


void modifyContentInFile(char *fileName,int lineNum,char *content)
{
	ifstream in;
	char line[1024]={'\0'};
	in.open(fileName);
	int i=0;
	string tempStr;
	while(in.getline(line,sizeof(line)))
	{
		i++;
		if(lineNum!=i)
		{
			tempStr+=charToStr(line);
		}
		else
		{
	       tempStr+=charToStr(content);
		}
		tempStr+='\n';
	}
	in.close();
	ofstream out;
	out.open(fileName);
	out.flush();
	out<<tempStr;
	out.close();
}


int main(int argc , char  * argv[])
{
	double alpha = atof(argv[1]);
	double epsilon = atof(argv[2]);
	double th = atof(argv[3]);

	//generate samples
    const int N = sample_gen(alpha, epsilon);

    //modify input size based on the number of samples
    char line[100];
    sprintf(line,"const int size = %d;",N);
    modifyContentInFile("./hls_kernel/inputdata.h", 4, line);

    //add the samples to csim tcl
    char linetcl[100];
    sprintf(linetcl, "add_files -tb %dsamples.txt", N);
    modifyContentInFile("run_hls_csim.tcl", 8, linetcl);

    //change the sample file to main.cpp
    char linemain[100];
    sprintf(linemain, "	FILE* f = fopen(\"%dsamples.txt\", \"r\");", N);
    modifyContentInFile("./hls_kernel/main.cpp", 16, linemain);

    //get the accurate result
    system("vivado_hls -f run_hls_csim.tcl");
 //   system("vivado_hls -f run_hls_csim.tcl");

    float ar[N];
 	float m;
    printf("Reading the accurate results\n");
	FILE* f1 = fopen("./float_test/solution1/csim/build/32", "r");
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
        //change bitwidth
        char changebit[100];
        sprintf(changebit, "const int F=%d;", F);
        modifyContentInFile("./hls_kernel/bit.h", 5, changebit);

        system("vivado_hls -f run_hls_csim.tcl");

        flag = verification(F, N, th, alpha, ar);

        if (flag == 0){
            break;
        }
	}

	bit = F + 1;
	printf("The verified fraction bit is: %d.\n", bit);

    modifyContentInFile("./hls_kernel/bit.h", 5, "const int F=23;");

    return 0;
}
