`ifndef LOG_AGENT_LITE_PKG_SV
 `define LOG_AGENT_LITE_PKG_SV
 
 package agent_lite_pkg;
 	import uvm_pkg::*;
	`include "uvm_macros.svh"
	//iclude agent components : driver, monitor, sequence
	import configurations_pkg::*; 
	`include "log_seq_item.sv"
	`include "driver1.sv"
	`include "monitor.sv"
	`include "sequencer.sv"
	`include "agent.sv"
 
 endpackage
 `endif
