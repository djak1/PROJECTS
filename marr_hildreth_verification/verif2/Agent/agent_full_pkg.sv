`ifndef LOG_AGENT_FULL_PKG_SV
 `define LOG_AGENT_FULL_PKG_SV
 
 package agent_full_pkg;
	import uvm_pkg::*;
	`include "uvm_macros.svh"
	//iclude agent components : driver, monitor, sequence
        import configurations_pkg::*; 
 	`include "log_seq_item_full.sv"
	`include "driver_full.sv"
	`include "monitor_full.sv"
	`include "sequencer_full.sv"
	`include "agent_full.sv"

 endpackage
`endif

