#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <CL/opencl.h>

#define DATA_SIZE 7

int values[] ={40,20,10,30,60,70,80};

extern "C" {
void process_top(int n, int *input, int *output, bool *fallback);
};

int
load_file_to_memory(const char *filename, char **result)
{
    size_t size = 0;
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
        {
            *result = NULL;
            return -1; // -1 means file opening fail
        }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    *result = (char *)malloc(size+1);
    if (size != fread(*result, sizeof(char), size, f))
        {
            free(*result);
            return -2; // -2 means file reading fail
        }
    fclose(f);
    (*result)[size] = 0;
    return size;
}

int main(int argc, char** argv)
{
#if defined(SDX_PLATFORM) && !defined(TARGET_DEVICE)
  #define STR_VALUE(arg)      #arg
  #define GET_STRING(name) STR_VALUE(name)
  #define TARGET_DEVICE GET_STRING(SDX_PLATFORM)
#endif
    //TARGET_DEVICE macro needs to be passed from gcc command line
    const char *target_device_name = TARGET_DEVICE;

    int err;                            // error code returned from api calls

    int input[DATA_SIZE];               // original data set given to device
    int device_results[DATA_SIZE];      // results returned from device
    int host_results[DATA_SIZE];        // results returned from host

    cl_platform_id platforms[16];       // platform id
    cl_platform_id platform_id;         // platform id
    cl_uint platform_count;
    cl_device_id device_id;             // compute device id
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel device_kernel;            // compute kernel

    char cl_platform_vendor[1001];

    cl_mem input_a;                     // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    cl_mem fallback;                    // device memory used for fallback

    // Fill our data sets with pattern
    //
    int i = 0;
    for(i = 0; i < DATA_SIZE; i++) {
        input[i] = values[i];
        device_results[i] = 0;
        host_results[i] = 0;
    }

    //
    // Get all platforms and then select Xilinx platform
    err = clGetPlatformIDs(16, platforms, &platform_count);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to find an OpenCL platform!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    printf("INFO: Found %d platforms\n", platform_count);

    // Find Xilinx Plaftorm
    int platform_found = 0;
    for (unsigned int iplat=0; iplat<platform_count; iplat++) {
        err = clGetPlatformInfo(platforms[iplat], CL_PLATFORM_VENDOR, 1000, (void *)cl_platform_vendor,NULL);
        if (err != CL_SUCCESS) {
            printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
        if (strcmp(cl_platform_vendor, "Xilinx") == 0) {
            printf("INFO: Selected platform %d from %s\n", iplat, cl_platform_vendor);
            platform_id = platforms[iplat];
            platform_found = 1;
        }
    }
    if (!platform_found) {
        printf("ERROR: Platform Xilinx not found. Exit.\n");
        return EXIT_FAILURE;
    }

    // Connect to a compute device
    // find all devices and then select the target device
    cl_device_id devices[16];  // compute device id
    cl_uint device_count;
    unsigned int device_found = 0;
    char cl_device_name[1001];
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR,
                         16, devices, &device_count);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to create a device group!\n");
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    //iterate all devices to select the target device.
    for (unsigned i=0; i<device_count; i++) {
        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 1024, cl_device_name, 0);
        if (err != CL_SUCCESS) {
            printf("Error: Failed to get device name for device %d!\n", i);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
        //printf("CL_DEVICE_NAME %s\n", cl_device_name);
        if(strcmp(cl_device_name, target_device_name) == 0) {
            device_id = devices[i];
            device_found = 1;
            printf("INFO: Selected %s as the target device\n", cl_device_name);
        }
    }

    if (!device_found) {
        printf("ERROR: Target device %s not found. Exit.\n", target_device_name);
        return EXIT_FAILURE;
    }


    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR,
                         1, &device_id, NULL);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to create a device group!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Create a compute context
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
        {
            printf("Error: Failed to create a compute context!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
        {
            printf("Error: Failed to create a command commands!\n");
            printf("Error: code %i\n",err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    int status;

    // Create Program Objects
    //

    // Load binary from disk
    unsigned char *kernelbinary;
    char *xclbin=argv[1];
    printf("INFO: Loading %s\n", xclbin);
    int n_i = load_file_to_memory(xclbin, (char **) &kernelbinary);
    if (n_i < 0) {
        printf("failed to load kernel from xclbin: %s\n", xclbin);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }
    size_t n = n_i;
    // Create the compute program from offline
    program = clCreateProgramWithBinary(context, 1, &device_id, &n,
                                        (const unsigned char **) &kernelbinary, &status, &err);
    if ((!program) || (err!=CL_SUCCESS)) {
        printf("Error: Failed to create compute program from binary %d!\n", err);
        printf("Test failed\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            printf("Error: Failed to build program executable!\n");
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
            printf("%s\n", buffer);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Create the compute kernel in the program we wish to run
    //
    device_kernel = clCreateKernel(program, "process_top", &err);
    if (!device_kernel || err != CL_SUCCESS)
        {
            printf("Error: Failed to create compute kernel!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Create the input and output arrays in device memory for our calculation
    //
    input_a = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * DATA_SIZE, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * DATA_SIZE, NULL, NULL);
    fallback = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(bool), NULL, NULL);
    if (!input_a || !output || !fallback)
        {
            printf("Error: Failed to allocate device memory!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Write our data set into the input array in device memory
    //
    err = clEnqueueWriteBuffer(commands, input_a, CL_TRUE, 0, sizeof(int) * DATA_SIZE, input, 0, NULL, NULL);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to write to source array a!\n");
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Set the arguments to our compute kernel
    //
    err = 0;
    int size = DATA_SIZE;
    err  = clSetKernelArg(device_kernel, 0, sizeof(size), &size);
    err |= clSetKernelArg(device_kernel, 1, sizeof(cl_mem), &input_a);
    err |= clSetKernelArg(device_kernel, 2, sizeof(cl_mem), &output);
    err |= clSetKernelArg(device_kernel, 3, sizeof(cl_mem), &fallback);
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to set kernel arguments! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    err = clEnqueueTask(commands, device_kernel, 0, NULL, NULL);
    if (err)
        {
            printf("Error: Failed to execute kernel! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }

    // Read back the results from the device to verify the output
    //
    cl_event readevent;
    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(int) * DATA_SIZE, device_results, 0, NULL, &readevent );
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to read output array! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    
    bool device_fallback = false;
    err = clEnqueueReadBuffer( commands, fallback, CL_TRUE, 0, sizeof(bool), &device_fallback, 0, NULL, &readevent );
    if (err != CL_SUCCESS)
        {
            printf("Error: Failed to read fallback info! %d\n", err);
            printf("Test failed\n");
            return EXIT_FAILURE;
        }
    if (device_fallback) printf("INFO: fallback to host");

    clWaitForEvents(1, &readevent);

    bool ignore;
    process_top(DATA_SIZE, input, host_results, &ignore);
    printf("\n\nOutput from host: ");
    for (i=0;i< DATA_SIZE;i++) {
        if (host_results[i] == -1) putchar('\n');
        else printf("%d\t",host_results[i]);
    }
    printf("\noutput end\n\n");

    printf("\n\nOutput from device: ");
    for (i=0;i< DATA_SIZE;i++) {
        if (device_results[i] == -1) putchar('\n');
        else printf("%d\t",device_results[i]);
    }
    printf("\noutput end\n\n");

    // Shutdown and cleanup
    //
    clReleaseMemObject(input_a);
    clReleaseMemObject(output);
    clReleaseKernel(device_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return EXIT_SUCCESS;
}
