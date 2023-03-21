`ifndef LOG_MONITOR_FULL_SV
 `define LOG_MONITOR_FULL_SV

class log_monitor_full extends uvm_monitor;
   bit checks_enable1 = 1;
   bit coverage_enable1 = 1;
   virtual interface axi_full_if vif;
   int 	   id;

   uvm_analysis_port #(log_seq_item_full) item_collected_port;
   log_seq_item_full log_item, clone_item;
   

   `uvm_component_utils_begin(log_monitor_full)
      `uvm_field_int(checks_enable1, UVM_DEFAULT)
      `uvm_field_int(coverage_enable1, UVM_DEFAULT)
      `uvm_field_int(id, UVM_DEFAULT) 
   `uvm_component_utils_end

   
   
   function new (string name = "log_monitor_full", uvm_component parent = null);
      super.new(name,parent);
      item_collected_port = new("item_collected_port", this);
   endfunction // new
   
   function void connect_phase(uvm_phase phase);
       super.connect_phase(phase);
       if (!uvm_config_db#(virtual axi_full_if)::get(this, "", "axi_full_if", vif))
         `uvm_fatal("NOVIF",{"virtual interface must be set:",get_full_name(),".vif"})
   endfunction // connect_phase
   

 task main_phase(uvm_phase phase);
    log_item = log_seq_item_full::type_id::create("log_item",this);
     //bit valid = 0;
      forever begin
	  if(vif.rst == 'b0) @(posedge vif.rst);
	
	  @(posedge vif.clk)
	   //axi write
	   if(vif.s01_axi_awvalid)begin
	      //valid = 'b1;
	      log_item.read_write = 0;
	 
	      //AXI Full
	      
	        log_item.addr1 = vif.s01_axi_awaddr;
	      		$display("Address := %d ", log_item.addr1);
	         log_item.length = vif.s01_axi_awlen;
	      		$display("Length := %d ", log_item.length);
	      //log_item.bsize = vif.s01_axi_awsize;
	      //log_item.btype = 1;
	        // item_collected_port.write(log_item);
	      

	      //get data
         	 $display("----------Data----------");
	      @(posedge vif.s01_axi_wvalid);
	      while(vif.s01_axi_wvalid) begin
		     if(vif.s01_axi_wready)begin
		       	
		       	log_item.data1 = vif.s01_axi_wdata;
		       	item_collected_port.write(log_item);
		       	$display("%d, ",log_item.data1);
		     end 
		     
		     @(posedge vif.clk);
		     if(vif.s01_axi_wlast)  break;
	      end
	      //wait response

	      while(vif.s01_axi_bvalid != 'b1 && vif.s01_axi_bready != 'b1) begin
		    @(posedge vif.clk);
	      end
	     
	      //$cast(clone_item, log_item.clone());
	         //item_collected_port.write(clone_item);
	      
	   end // if (vif.s01_axi_awvalid)
	   else if(vif.s01_axi_arvalid)begin
          log_item.read_write  = 1;
	    // get read address
	      log_item.addr1   = vif.s01_axi_araddr;
	      $display("Address := %d ", log_item.addr1);
	      log_item.length = vif.s01_axi_arlen;
	      $display("Length := %d ", log_item.length);
	      //log_item.bsize  = vif.s01_axi_arsize;
		  //log_item.btype = 1;
          
          $display("----------Data----------");
	      @(posedge vif.s01_axi_rvalid);
	      while (vif.s01_axi_rvalid)begin
		      if(vif.s01_axi_rready) begin
		          log_item.data1 = vif.s01_axi_rdata;
		          $display("%d, ",log_item.data1);
		      end
		      @(posedge vif.clk);
		      if(vif.s01_axi_rlast)break;
	      end
	   end // if (vif.s01_axi_arvalid)
	   else begin
	      //valid = 'b0;
	   end // else: !if(vif.s01_axi_arvalid)
	 	 
      end // forever begin

 endtask // main_phase
   
endclass // log_monitor


`endif
