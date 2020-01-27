# Probablistic Verification
This is the implementation of differential execution-based probabilistic verification to select the reduced bitwidth for floating point. 

## Prerequisites
- To run the code, you must have a valid Xilinx Vivado HLS installed.
- If you do not have a Xilinx Vivado HLS environment but you are interested in how to reproduce the results manually, please go to "Manual Verification" folder in each subject program, and follow the instructions there.

## How to use this tool
- To reproduce the results, please go to folder KNN-l2norm and RGB2YUV
- To run a new kernel program, please go to folder Probabilistic_Verification

## folder structure
- KNN-l2norm
  - hls_kernel: This contains the original kernel program.
  - Refactored Kernel: This contains the refactored kernel program.
  - Manual Verification: If you do not have a valid Xilinx Vivado HLS environment, this folder contains the ready-to-use HLS csim outputs. You can verify the reduced bitwidths manually based on these outputs.
  - Report: If you do not have a valid Xilinx Vivado HLS environment, this folder contains the synthesis reports for the original and refactored versions of the benchmark.
- RGB2YUV
  - hls_kernel
  - Refactored Kernel
  - Manual Verification
  - Report
- Probabilistic_Verification
- README.md
