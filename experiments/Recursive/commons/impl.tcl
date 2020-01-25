open_project device -reset
add_files kernel.cpp
set_top process_top
open_solution -reset device
set_part xcvu9p-fsgd2104-3-e
create_clock -period 3.3
csynth_design
export_design -flow impl -rtl verilog -format ip_catalog
close_project
exit
