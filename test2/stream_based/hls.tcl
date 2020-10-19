open_project prj_stream_based
set_top top_stream
add_files dut_stream.cpp
add_files sw_reorder.cpp
add_files dut_stream.hpp
add_files -tb main.cpp
open_solution "solution1"
set_part {xcvu11p-flga2577-1-e}
create_clock -period 10 -name default
csim_design
csynth_design
cosim_design
exit
#export_design -format ip_catalog
