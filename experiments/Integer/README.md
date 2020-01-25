# Integer Bitwidth Optimization
This contains the steps to instrument the benchmarks and automated refactoring to reduce the integer bitwidth.

## Steps
- Instrumentation of the benchmark using https://plse.cs.washington.edu/daikon/download/doc/daikon/Front-ends-_0028instrumentation_0029.html#Kvasir.
- Eclipse plugin to identify optimized bitwidth based on instrumentation results.
- Clang-based Automated Refactoring to reduce the bitwidth and insert guard conditions.

## Prerequisites for 
- To run the instrumentation you should have kvasir installed on your machine
- To run the invariant identificaiton you should have eclipse plugin installed on your machine
- To run automated refactoring, you must have llvm installed in your machine

## VM
- You can find a VM here <> with required pre-reqs installed. 
### How to run


## folder structure
- Benchmark Program:
    - Instrumentation
        - input
        - kvasir output
        - invariants
            -- invariants.txt -- (funcname:variablename, bitwidth, type signature)
            -- invariant_values.txt -- (funcname:variablename, min and max values, number of unique values (in case of arrays))
    - src
    - transformed
    - reports
        - Contains synthesis reports for the original and refactored versions of the benchmark

