# Experiment: RGB2YUV

## Automatic Probabilistic Verification
This requires a valid Xilinx Vivado HLS environment.
- Go to the current directory, type the following command
  - cmake .
  - make
  - ./fpverification alpha epsilon error. For example: "./fpverification 0.999 0.03 0.000001"

## Manual Verification
if you do not have a valid Xilinx Vivado HLS environment, please go to the Manual Verification folder. We have provided all the ready-to-use hls csim outputs and you can verify the bitwidth manually based on those outputs.
- Please note that the verified bitwidth will be different from Table 4 in the ICSE submission version. This is because the reviewers said the bitwidth does not change for different confidence level alpha, and we have then updated the kernel program. The kernel programs here are the updated version.



