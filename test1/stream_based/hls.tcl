
open_project prj
set_top krnl_dct
add_files dct.cpp
add_files -tb main.cpp

open_solution "solution"
set_part {xcu200-fsgd2104-2-e}
create_clock -period 3.333333 -name default
config_flow -target vitis
csim_design -argv {32400}
csynth_design
cosim_design -argv {32400}
exit
