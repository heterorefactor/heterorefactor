# Experiment: KNN-l2norm

## Automatic Probabilistic Verification
This requires a valid Xilinx Vivado HLS environment.
- Go to the current directory, type the following command
  - cmake .
  - make
  - ./fpverification alpha epsilon error. For example: "./fpverification 0.999 0.03 0.000001"

## Manual Verification
if you do not have a valid Xilinx Vivado HLS environment, please go to the Manual Verification folder. We have provided all the ready-to-use hls csim outputs and you can verify the bitwidth manually based on those outputs.
- Please note that the verified bitwidth will be different from Table 4 in the ICSE submission version. This is because the reviewers said the bitwidth does not change for different confidence level alpha, and we have then updated the kernel program. The kernel programs here are the updated version.
- For example, to reproduce line 1 in Table 4, please go to ./Manual Verification/0.95
  - Type the following command: "cmake .", "make"
  - type "./fpmanual 0.95 0.03 0.000001". You can also change the last argument to be 10^(-2) or 10^(-4). In Table 4, epsilon (the second argument) is fixed to be 0.03, there is no need to change this argument.
- For example, to reproduce line 3 in Table 4, please go to ./Manual Verification/0.999
  - Type the following command: "cmake .", "make"
  - type "./fpmanual 0.999 0.03 0.000001". You can also change the last argument to be 10^(-2) or 10^(-4). In Table 4, epsilon (the second argument) is fixed to be 0.03, there is no need to change this argument.

# HLS Report
After synthesizing, Xilinx vivado hls will generate a report on resource utilization automatically. Numbers in Table 1 are from these reports.



