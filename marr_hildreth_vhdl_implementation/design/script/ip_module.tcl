variable dispScriptFile [file normalize [info script]]

proc getScriptDirectory {} {
    variable dispScriptFile
    set scriptFolder [file dirname $dispScriptFile]
    return $scriptFolder
}

set sdir [getScriptDirectory]
cd [getScriptDirectory]

set resultDir ../hdl/result/ip_module
file mkdir $resultDir

create_project laplacian_zero ../opp/result/laplacian \
-part xc7z020clg484-1 -force

add_files -norecurse ../hdl/rtl/ip_module_v1_0.vhd
add_files -norecurse ../hdl/rtl/ip_module_v1_0_S00_AXI.vhd
add_files -norecurse ../hdl/rtl/ip_module_v1_0_S01_AXI.vhd
add_files -norecurse ../hdl/rtl/laplacian.vhd
add_files -norecurse ../hdl/rtl/memory_beh.vhd
add_files -norecurse ../hdl/rtl/mem_subsystem.vhd
add_files -norecurse ../hdl/rtl/top.vhd
add_files -norecurse ../hdl/rtl/utils_pkg.vhd
add_files -norecurse ../hdl/rtl/zero.vhd
add_files -norecurse ../hdl/tb/ip_module_v1_0_tb.vhd

add_files -fileset constrs_1 ../hdl/xdc/ip_module.xdc

update_compile_order -fileset source_1

launch_runs synth_1
wait_on_run synth_1

puts "*****************************************************"
puts "* Sinteza zavrsena! *"
puts "*****************************************************"
