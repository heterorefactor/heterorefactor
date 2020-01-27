# HeteroRefactor (icse2020-artifacts)
Artifacts submission for ICSE 2020.

## Prerequisites

We assume Ubuntu 16.04.5 LTS is installed. The following packages are required to be installed before building this system. We list them as Ubuntu package names. If you are using another operating system, please check the corresponding package names by yourself.

```bash
sudo apt-get install gawk git wget tar bzip2 gcc automake autoconf \
    libhpdf-dev libc6-dev autotools-dev bison flex libtool libbz2-dev \
    libpython2.7-dev ghostscript libhpdf-dev libmpfrc++-dev
```

To get the resource utilization results for the original and refactored kernels, a valid Xilinx Vivado license is required to do the FPGA synthesis and implementation. Please make sure they are in your `PATH` environment variable, and you can run `vivado` and `vivado_hls` in your terminal. 

We made some modifications on the library that is shipped with your Vivado installation to get / reproduce the results for floating-point kernels. Due to copyright issues, we cannot release the modified code in public. This library will be available upon request if you have a valid license. Please send an email to Jason Lau \<<lau@cs.ucla.edu>\> along with a screenshot of the license screen of your Vivado installation. We will reply with the code and instructions as soon as possible we receive and verify the request.

## How to build the HeteroRefactor tool

Simply `make`! If you only want to build the tool without running tests:

```
cd heterorefactor; make
```

Alternatively, you can build all the required libraries:

```bash
cd heterorefactor/libraries; make
```

Then build the refactoring tool:

```bash
cd heterorefactor/refactoring; make
```

You can add `-j 16` option to the `make` command for faster building. Adjust the
number `16` to match your core numbers on your system.

## How to Use HeteroRefactor

After building the system, the tool `heterorefactor` is available at `heterorefactor/refactoring/build/heterorefactor`. For your convenience of usage, we hardcoded a relative path to avoid you explicitly specifying the root path of this project. Therefore, please do not move the binary file. Optionally, you can add `heterorefactor/refactoring/build/` to your `PATH`.

The usage follows. You can check `heterorefactor -h` for a detailed manual.

```bash
heterorefactor [-int/-fp/-rec/-instrument] \
    -I path/your/include/files [...and other GCC compiler options] \
    -u refactored_output_code.cpp \
    input_code.cpp
```

For example, if you want to refactor the linked list kernel using HeteroRefactor and output to `output.cpp`:

```bash
heterorefactor -rec -u output.cpp experiments/Recursive/ll/src/kernel.cpp
```

You can see more detailed usage, including how to obtain and use the invariants, in each experiment folder.

## How to Reproduce Paper Results

See `README.md` files under `experiments/FP`, `experiments/Integer` and `experiments/Recursive`.

## Project Folder Structure

- **heterorefactor**
  - **libraries**: building scripts and patches for all libraries
    - **rose-compiler**: automated compilation of ROSE
    - **xilinx-dummy-lib**: a modified Xilinx libraries for ROSE, AVAILABLE UPON REQUEST
    - **template-hls-float.patch**: patch file for the floating point library
  - **refactoring**: the code of HeteroRefactor: ROSE-based refactoring
  - **instrumentation**: resource files to perform instrumentation. see experiments for detail.
- **experiments**
  - **FP**
  - **Integer**
  - **Recursive**

## TODOs before opensource releasing
- Remove the modified Xilinx library `heterorefactor/libraries/xilinx-dummy-lib/`.
