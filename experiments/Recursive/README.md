## Experiments on recursive data structure kernels

This document contains the information to reproduce the experimental results on
recursive data structure kernels. We will first provide instructions to run
several one-click scripts to reproduce the report files and explain how to
interpret them. Then we will discuss the underlying steps in these scripts.
Finally, we will present how to add a new kernel to these experiments.

All commands in this document are assumed to be executed in the directory of
this README file. If you changed the directory in one step, please change it
back in the following steps.

### Prerequisites

#### System packages

We assume Ubuntu 16.04.5 LTS is installed. All prerequisites in the root
README.md file should be installed:

```bash
sudo apt-get install gawk git wget tar bzip2 gcc automake autoconf \
    libhpdf-dev libc6-dev autotools-dev bison flex libtool libbz2-dev \
    libpython2.7-dev ghostscript libhpdf-dev libmpfrc++-dev
```

Additionally, if you want to reproduce the synthesis and implementation results,
you need to have a valid Vivado license and install that on your system. You
need to make sure that they are in your `PATH` environment variable, and you can
run `vivado` and `vivado_hls` in your terminal.

#### HeteroRefactor tool

The heterorefactor tool should be built to run all the experiments:

```bash
cd ../heterorefactor; make
```

Alternatively, you can build all the required libraries:

```bash
cd ../heterorefactor/libraries; make
```

Then build the refactoring tool:

```bash
cd ../heterorefactor/refactoring; make
```

You can add `-j 16` option to the `make` command for faster building. Adjust the
number `16` to match your core numbers on your system.

### One-click Script to Reproduce Results

We have generated all results on our server and they are already ready for
checking under this folder. See How to Interpret. If you want to generate them
again in your environment, use `make clean` under this folder to clean all
generated results first.

You can reproduce the invariant dectection and refactoring with:

```bash
make
```

If you have Vivado and Vivado HLS installed on your system, you can then
generate synthesis report for resource estimation and synthesizablity report
using:

```bash
make synthesis
```

If you have Vivado and Vivado HLS installed on your system, you can also
generate an implementation report for frequency and an actual resource report:

```bash
make implement
```

### How to Interpret

All the refactored code are available under `[kernel]/refactored/`:

* `[kernel]/refactored/kernel_instrument.cpp` is the automatically generated
  instrumentation code for invariant detection.
* `[kernel]/refactored/kernel_device_[size].cpp` is the automatically refactored
  kernel code that can be synthesized with Vivado HLS. It is generated using the
  invariant detected by the instrumentation code with an input size of `[size]`.

You can check `[kernel]/invariant/` for detected invariants.

For data in the figure showing operating frequency for different input sizes,
and also the table showing resource utilization for HeteroRefactor, use command:

```bash
make report
```

This command reads from `*/rpt/implement_report*` and summarizes the results into
a single CSV file. The clock period is shown in ns, e.g., 5ns clock period = 100
MHz frequency. You may also read from `[kernel]/rpt/implement_report_[size].txt`
to interpret them on your own. You need to have implementation report before this
step. They are already available in this repository if you did not perform
`make clean`, or if you have successfully completed `make implement`.

There are some other reports available under `[kernel]/rpt/`:

* `[kernel]/rpt/synthesis_log_original.txt` shows the error output by Vivado HLS
  during the synthesis of the original kernel before our refactoring.
* `[kernel]/rpt/implement_log_[size].txt` shows the log output by Vivado during
  implementation of the kernel of size `[size]`.
* `[kernel]/rpt/implement_time_[size].txt` shows the implementation time of the
  kernel of size `[size]`.
* `[kernel]/rpt/original_run_time_[size].txt` shows the original run time.
* `[kernel]/rpt/instrument_run_time_[size].txt` shows the run time of the
  instrumentation code of a certain kernel size.
* `[kernel]/rpt/refactoring_time_[size].txt` shows the refactoring time of a
  kernel of size `[size]`.
* Especially, `ac/rpt/implement_raw_vivado_bug_32768.txt` shows the raw report
  file produced by Vivado instead of Vivado HLS. Because Vivado HLS has a bug
  that will result in a zero resource utilization report for some kernels, we
  extract the raw report file for your reference.

### What Happened Behind

You can always check the `Makefile` to see what is performed. For your
convenience, we present the overall reproduce steps. You can also perform these
steps manually.

#### Invariant Detection

We use the `instrument` mode of HeteroRefactor to refactor the kernel code to
produce a profile file at a given location, then compile and run with a testdata
with the given size. We parse the profile file to produce an invariant file as the
new input of HeteroRefactor for `rec` mode. This is the target `invariant` in
the Makefile. It performs the following operations:

First, it calls `heterorefactor` to generate the code with instrumentation. This
step can be timed with `time` command.

```bash
../heterorefactor/refactoring/build/heterorefactor \
    -instrument /tmp/profile-output \
    -u [kernel]/refactored/kernel_instrument.cpp
    [kernel]/src/kernel.cpp
```

Then, it compiles the instrumented kernel code with the given testbed for a
profile run:

```bash
g++ -o [kernel]/bin/instrument.exe \
    [kernel]/refactored/kernel_instrument.cpp \
    [kernel]/src/testbed.cpp
```

It executes the compiled binary to generate profile output. In order to give it
an input of size `[size]`, it calls the provided data generator:

```bash
[kernel]/testdata/data_generator.sh [size] | \
    [kernel]/bin/instrument.exe
```

Finally, the profile file is parsed to detect invariant:

```bash
awk -f ../heterorefactor/instrumentation/recursive/generate_invariant.awk \
    /tmp/profile-output > [kernel]/invariant/invariant_[size].ivr
```

The invariant file is now available at
`[kernel]/invariant/invariant_[size].ivr`.

#### Refactoring

We use the invariant detected in the previous step for the `rec` mode of
HeteroRefactor to refactor the kernel code to enable Vivado HLS synthesis.
This is the target `refactor` in the Makefile. 

```bash
../heterorefactor/refactoring/build/heterorefactor \
    -rec -recfile [kernel]/invariant/invariant_[size].ivr
    -u [kernel]/refactored/kernel_device_[size].cpp
    [kernel]/src/kernel.cpp
```

The HeteroRefactor tool will produce
`[kernel]/refactored/kernel_device_[size].cpp` as the refactored kernel code.
Note that this code can not only be synthesized, but also be executed on CPU.
Therefore, you can always use `g++` to verify the correctness:

```bash
g++ [kernel]/refactored/kernel_device_[size].cpp [kernel]/src/testbed.cpp
[kernel]/testdata/data_generator.sh [size] | ./a.out
```

#### Synthesis and Implementation

In HLS terminology, synthesis means transform the input C++ code into Verilog
code in [RTL level](https://en.wikipedia.org/wiki/Register-transfer_level).
This transformed Verilog code can be implemented into a bitstream, i.e., a
binary file that can be executed on FPGA.

In order to synthesize the refactored kernel for execution on FPGA, we use the
Tcl script `commons/hls.tcl` to instruct Vivado HLS. This is the target `synthesis`
in the Makefile. It reads a file
`kernel.cpp` and sets `process_top` as the top-level function; it opens a
solution called `device`; it sets the target FPGA as `xcvu9p-fsgd2104-3-e` and
the target clock period as `3.3ns`; it synthesizes the code then close the
project:

```tcl
open_project device -reset
add_files kernel.cpp
set_top process_top
open_solution -reset device
set_part xcvu9p-fsgd2104-3-e
create_clock -period 3.3
csynth_design
close_project
exit
```

To use this script, we copy the kernel file to the same directory as this Tcl
script, rename it to `kernel.cpp` then call Vivado HLS:

```bash
cp [kernel]/refactored/kernel_device_[size].cpp \
    [kernel]/tmp/synthesis_[size]/kernel.cpp
cp commons/hls.tcl [kernel]/tmp/synthesis_[size]/hls.tcl
cd [kernel]/tmp/synthesis_[size]/
vivado_hls -f hls.tcl
```

It will generate a log file at `[kernel]/tmp/synthesis_[size]/vivado_hls.log`
and also a report file with resource estimation at
`[kernel]/tmp/synthesis_[size]/device/device/syn/report/process_top_csynth.rpt`.

If you want to verify that the original kernel is not synthesizable, you can
replace `[kernel]/refactored/kernel_device_[size].cpp` with
`[kernel]/src/kernel.cpp` on the commands above. You can see the error messages
in `vivado_hls.log`.

To perform implementation, simply add a Tcl command in the `hls.tcl` file right
after `csynth_design`:

```tcl
...
csynth_design
export_design -flow impl -rtl verilog -format ip_catalog
close_project
exit
```

Alternatively, you can use `commons/impl.tcl`. This is the target `implement`
in the Makefile.

```bash
cp [kernel]/refactored/kernel_device_[size].cpp \
    [kernel]/tmp/implement_[size]/kernel.cpp
cp commons/impl.tcl [kernel]/tmp/implement_[size]/hls.tcl
cd [kernel]/tmp/implement_[size]/
vivado_hls -f impl.tcl
```

It will generate a log file also called `vivado_hls.log`. The report file with
final resource ultilization is at
`device/device/impl/report/verilog/process_top_export.rpt`. Alternatively, you
can see `device/device/impl/verilog/report/process_top_utilization_routed.rpt`
for the raw result from Vivado without parsing by Vivado HLS. For AC kernel of
size 32768, you need to use this file for results due to a Vivado HLS bug.

If you are a GUI user, you can simply add the corresponding
`kernel_device_[size].cpp` file to your project and perform synthesis or
implementation on the toolbar. The report file should be displayed on the
screen. However, the author of this document has not tested in a GUI way.

### How to Add New Kernels

If you want to add your own new kernels for testing, you can create a new
directory under this folder. You can create this subfolder structure:

```
├── Makefile
├── src
│   ├── kernel.cpp
│   └── testbed.cpp
└── testdata
    └── data_generator.sh
```

You can directly copy the Makefile from `ac/Makefile`. The `kernel.cpp` should
define an HLS kernel named `process_top` with proper HLS interface annotation.
The `testbed.cpp` should contain a `main` function to read input data from
stdin, use `process_top` to process this data and possibly output to stdout. The
`data_generator.sh` should accept one argument as the data size, and output a
valid input data for the testbed with the given size to stdout. You can always
refer to the existing kernels as an example.

After these steps, you can call `make`, `make synthesis` and `make implement`
under your own directory. It will produce the results as for other kernels.

---

![vast](http://vast.cs.ucla.edu/sites/default/themes/CADlab_cadlab/images/logo.png)
