`ifndef LOG_SEQ_PKG_SV
 `define LOG_SEQ_PKG_SV
package log_seq_pkg;
	import uvm_pkg::*;
	`include "uvm_macros.svh"	
	
	import agent_full_pkg::log_seq_item_full;
	import agent_full_pkg::log_sequencer_full;
	
	import agent_lite_pkg::log_seq_item;
	import agent_lite_pkg::log_sequencer;
        
        
        `include "base_lite_seq.sv"
	`include "base_full_seq.sv"
	`include "simple_lite_seq.sv"
        `include "simple_full_seq.sv"
        
	`include "virtual_sequence.sv"
endpackage: log_seq_pkg;
`endif
