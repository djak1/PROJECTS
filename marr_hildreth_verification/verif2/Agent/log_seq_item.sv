`ifndef LOG_SEQ_ITEM_SV
 `define LOG_SEQ_ITEM_SV

class log_seq_item extends uvm_sequence_item;
	
	   parameter WIDTH = 8;
	   parameter SIZE = 10;
	   parameter WIDTH_KERNEL = 5;
	   parameter SIZE_KERNEL = 5;
	   //-- Parameters of Axi Slave Bus Interface 
	   parameter C_S00_AXI_DATA_WIDTH    = 32;
	   parameter C_S00_AXI_ADDR_WIDTH = 3;
	    //-- Parameters of Axi Slave Bus Interface S01_AXI
	
		
           //Axi lite
            rand bit [C_S00_AXI_DATA_WIDTH-1 : 0]           addr; 
            rand bit [C_S00_AXI_DATA_WIDTH-1: 0] 	       data; //logic
            rand bit 	                                      read_write;
            rand bit avalid;
            rand bit dvalid;
            
           
  
 `uvm_object_utils_begin(log_seq_item)
	`uvm_field_int(addr, UVM_DEFAULT)
        `uvm_field_int(data, UVM_DEFAULT)
        `uvm_field_int(read_write, UVM_DEFAULT)
        `uvm_field_int(avalid, UVM_DEFAULT)
        `uvm_field_int(dvalid, UVM_DEFAULT)
             
`uvm_object_utils_end


    constraint address_constraint {addr inside {1, 4, 8, 12, 16, 20, 24};}
    constraint data_constraint {data <= 150;}


   
 function new(string name = "log_seq_item");
 	super.new(name);
 endfunction //new
 
endclass : log_seq_item

`endif



















