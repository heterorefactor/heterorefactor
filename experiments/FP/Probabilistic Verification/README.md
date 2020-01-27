# Probablistic Verification
This is the implementation of diffiential execution-based probabilistic verification to select the reduced bitwidth for floating point. 

## Prerequisites
- To run the code, you must have a valid Xilinx Vivado HLS installed.
- If you do not have a Xilinx Vivado HLS environment but you are interested in how to reproduce the results manually, please go to the "Manual Verification" folder, and follow the instructions there.

## folder structure
- Manual Verification
- Subject Programs
- 

## How to run the probablistic verification
To reproduce the results of RGB2YUV:
1. open Terminal and type the command: g++ main.cpp sampleGen.cpp verification.cpp
2. type the command: ./a.out alpha epsilon error

