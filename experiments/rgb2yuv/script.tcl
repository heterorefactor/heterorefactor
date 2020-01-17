open_project kernel
set_top rgb2yuv
add_files rgb2yuv.cpp -cflags "-std=c++11 -fexceptions -I../../heterorefactor/libraries/template-hls-float/include/"
add_files -tb testbed.cpp
add_files -tb main.cpp
open_solution "solution"
set_part {xcvu9p-flgb2104-2-i} -tool vivado
create_clock -period 4 -name default
config_compile -name_max_length 50  -pipeline_loops 0 -unsafe_math_optimizations
csim_design
#csynth_design
