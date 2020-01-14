# icse2020-artifacts
Artifacts submission for ICSE 2020.

## Prerequisites

The following packages are required to be install before building this system (listed as Ubuntu package name):

```
sudo apt-get install git wget tar bzip2 gcc automake autoconf \
    autotools-dev bison flex libtool libbz2-dev libpython2.7-dev ghostscript
```

## Tentative folder structure scheme

- heterorefactor
  - libraries (JL: building scripts and patches for all libraries)
    - Makefile
    - rose-compiler (JL: automated compilation for ROSE)
    - xilinx-dummy-lib (JL: dummy include files of Xilinx libraries optimized for ROSE)
    - template-hls-float.patch (JL: patch file for the floating point library)
  - instrumentation
    - recursive
    - integer
    - fp
  - refactoring
    - recursive
    - integer
    - fp
- experiments
  - ac
    - makefile
    - src
      - host_main.cpp
      - kernel.cpp
      - kernel_hr_refactored.cpp
      - kernel_manual_refactored.cpp
    - input
      - input_file.txt
    - invariant
      - instrumentation.out
    - rpt
      - hls_report.rpt
      - (if in paper) vivado_report.rpt
  - dfs
  - ll
  - merge
  - strassen
  - fd
  - 3d
  - bubble
  - knn
  - rgb2yuv

## TODOs before opensource releasing

- Remove the modified Xilinx library `heterorefactor/libraries/xilinx-dummy-lib/`.
    - This file is required for reproducing refactoring results since the original Xilinx library cannot be parsed by ROSE. We removed some irrelevant code and only keep the framework for ROSE analysis.
    - However, this dummy skeleton library contains some of Xilinx proprietary code and APIs design. These code must not be distributed without a valid license agreement with Xilinx.
    - Therefore, while it is OK for artifacts review, we need to remove this library before releasing this repository to the public.
