## How to perform instrumentation for recursive data structures

The instrumentation for recursive data structures is refactoring-based and
relies on the refactoring part of heterorefactor to work.
See /heterorefactor/refactoring for details.

To prepare a kernel for instrumentation, run the following command:

```
path/to/heterorefactor -instrument path/to/profile_file -u instrument.cpp kernel.cpp
```

Then you can compile your program to use `instrument.cpp` in place of `kernel.cpp`.
By running the program as you will do with your old kernel file, a profile file
`path/to/profile_file` will be generated with all traces of your kernel.

To generate invariant from instrumentation profile file, run the following command:

```
awk -f path/to/generate_invariant.awk path/to/profile_file > path/to/invariant
```

After these steps, you can refactor your program with the invariant file:

```
path/to/heterorefactor -rec -recfile path/to/invariant -u refactored.cpp kernel.cpp
```
