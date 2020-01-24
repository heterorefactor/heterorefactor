#include <iostream>
#include <string>
#include <vector>
#include "kernel.h"
#include "time.h"
#include <sys/time.h>

#include "CLWorld.h"
#include "CLKernel.h"
#include "CLMemObj.h"
using namespace rosetta;

int main(int argc, char** argv) {
    std::string kernelFile(argv[1]);
    int n = atoi(argv[2]),
        l = atoi(argv[3]),
        m = atoi(argv[4]);

    srand(time(NULL));
	int* matA = new int[n * l];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < l; j++) {
            matA[i * l + j] = rand() % 100;
        }
    }

	int* matB = new int[l * m];
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < m; j++) {
            matB[i * m + j] = rand() % 100;
        }
    }

	int* matC = new int[n * m];

	int *fallback = new int[1];

    CLWorld world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);
    world.addProgram(kernelFile);

    CLKernel Kernel(world.getContext(), world.getProgram(),
            "process_top", world.getDevice());

    CLMemObj n_mem((void*)&n, sizeof(int), 1, CL_MEM_READ_ONLY);
    CLMemObj l_mem((void*)&l, sizeof(int), 1, CL_MEM_READ_ONLY);
    CLMemObj m_mem((void*)&m, sizeof(int), 1, CL_MEM_READ_ONLY);
    CLMemObj matA_mem((void*)matA, sizeof(int), n*l, CL_MEM_READ_ONLY);
    CLMemObj matB_mem((void*)matB, sizeof(int), l*m, CL_MEM_READ_ONLY);
    CLMemObj matC_mem((void*)matC, sizeof(int), n*m, CL_MEM_WRITE_ONLY);
    CLMemObj fallback_mem((void*)fallback, sizeof(int), 1, CL_MEM_WRITE_ONLY);

    struct timeval start, end;
    gettimeofday(&start, 0);

    world.addMemObj(n_mem);
    world.addMemObj(l_mem);
    world.addMemObj(m_mem);
    world.addMemObj(matA_mem);
    world.addMemObj(matB_mem);
    world.addMemObj(matC_mem);
    world.addMemObj(fallback_mem);

    int global_size[3] = {1, 1, 1};
    int local_size[3] = {1, 1, 1};
    Kernel.set_global(global_size);
    Kernel.set_local(local_size);
    world.addKernel(Kernel);

    world.setMemKernelArg(0, 0, 0);
    world.setMemKernelArg(0, 1, 1);
    world.setMemKernelArg(0, 2, 2);
    world.setMemKernelArg(0, 3, 3);
    world.setMemKernelArg(0, 4, 4);
    world.setMemKernelArg(0, 5, 5);
    world.setMemKernelArg(0, 6, 6);

    world.runKernels();

    world.readMemObj(6);

	if (fallback[0]) {
        struct timeval start, end;
        gettimeofday(&start, 0);
		printf("INFO: falling back!\n");
        process_top(&n, &l, &m, matA, matB, matC, fallback);
        gettimeofday(&end, 0);
        long long elapsed = (end.tv_sec - start.tv_sec) * 1000000LL + end.tv_usec - start.tv_usec;
        fprintf(stderr, "%lld,", elapsed);
	} else {
        world.readMemObj(5);
        world.releaseWorld();
    }

    gettimeofday(&end, 0);
    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000LL + end.tv_usec - start.tv_usec;

    fprintf(stderr, "%lld\n", elapsed);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
            int correct = 0;
            for (int k = 0; k < l; k++) {
                correct += matA[i * l + k] * matB[k * m + j];
            }
            if (matC[i * m + j] != correct) {
                printf("ERROR: %d should be %d\n", matC[i * m + j], correct);
            }
		}
	}

	return 0;
}
