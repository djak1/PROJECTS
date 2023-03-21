`ifndef LOG_MONITOR_SV
 `define LOG_MONITOR_SV

class log_monitor extends uvm_monitor;
   //Control fileds
   bit checks_enable = 1;
   bit coverage_enable = 1;

   virtual interface axi_lite_if vif;
   int 	   id;

   uvm_analysis_port #(log_seq_item) item_collected_port;
   
   log_seq_item log_item, item_clone;
   

   `uvm_component_utils_begin(log_monitor)
      `uvm_field_int(checks_enable, UVM_DEFAULT)
      `uvm_field_int(coverage_enable, UVM_DEFAULT)
      `uvm_field_int(id, UVM_DEFAULT) 
   `uvm_component_utils_end

   
   
   function new (string name = "log_monitor", uvm_component parent = null);
      super.new(name,parent);
      //log_item = new();
      item_collected_port = new("item_collected_port", this);
   endfunction // new
   
   function void connect_phase(uvm_phase phase);
       super.connect_phase(phase);
       if (!uvm_config_db#(virtual axi_lite_if)::get(this, "", "axi_lite_if", vif))
         `uvm_fatal("NOVIF",{"virtual interface must be set:",get_full_name(),".vif"})
   endfunction // connect_phase
   

   
   task main_phase(uvm_phase phase);
     //log_item = log_seq_item::type_id::create("log_item",this);
    
    
     forever begin
	
	 if (vif.rst == 'b0) @(posedge vif.rst);
	 if (vif.s00_axi_awvalid == 'b1) begin
	    
	   log_item = log_seq_item::type_id::create("log_item",this);
	   
	   log_item.read_write = 0; //write	    
	   log_item.addr  = vif.s00_axi_awaddr[4:0];
	   $display("Colected ...: Address :=  %d \n", log_item.addr);
	   @(negedge vif.s00_axi_wvalid);
	   log_item.data  = vif.s00_axi_wdata;
	   $display("Colected ...: Data :=  %d \n", log_item.data);
	   @(posedge vif.s00_axi_wvalid);
	   
	
	   $cast(item_clone, log_item.clone());
	   item_collected_port.write(item_clone);

	 end
	 else if (vif.s00_axi_arvalid == 'b1) begin
	   log_item = log_seq_item::type_id::create("log_item",this);
	   log_item.read_write = 1;	    //read
	   log_item.addr  = vif.s00_axi_araddr[4:0];
	   $display("Colected ...: Address :=  %d \n", log_item.addr);
	   @(negedge vif.s00_axi_rvalid);
	   log_item.data  = vif.s00_axi_rdata;
	   $display("Colected ...: Data :=  %d \n", log_item.data);
	   @(posedge vif.s00_axi_rvalid);
	
	   $cast(item_clone, log_item.clone());
	   item_collected_port.write(item_clone);
	   
	 end
	 @(posedge vif.clk);
	
	
	
     end

   endtask // main_phase



 
     
endclass // log_monitor


`endif
