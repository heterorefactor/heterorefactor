# Probablistic Verification
This is the implementation of diffiential execution-based probabilistic verification to select the reduced bitwidth for floating point. 

## How to run the probablistic verification
If you have a valid Xilinx Vivado HLS environment:
1. open Terminal, go to the current directory and type the command: g++ main.cpp sampleGen.cpp verification.cpp
2. type the command: ./a.out alpha epsilon error. For example: ./a.out 0.999 0.03 0.000001

## folder structure
- hls_kernel: This contains the specific program that we will work on to determine the bitwidth based on differential execution.
- subject programs: Two benchmarks for floating point.
- README.md

## To run a new benchmark
- Put your program files in the hls_kernel folder;
- Put your run_hls_csim.tcl file in the current directory;
- Provide the specific sampleGen function in sampleGen.cpp that satisfies the data format for your program;
- Compile and run.



