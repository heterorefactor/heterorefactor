open_project float_test

set_top rgb2yuv

add_files hls_kernel/rgb2yuv.cpp -cflags "-I ../../../heterorefactor/libraries/template-hls-float/include -DTHLS_FW_UINT_ON_AP_UINT=1 -std=c++11 -fexceptions"
add_files -tb hls_kernel/main.cpp
add_files -tb hls_kernel/testbed.cpp
add_files -tb 4223samples.txt

open_solution "solution1"
set_part {xcvu9p-fsgd2104-3-e} -tool vivado
create_clock -period 10 -name default

csim_design -O
#csynth_design
#cosim_design
#export_design -format ip_catalog
exit

