`ifndef LOG_DRIVER_SV
 `define 	LOG_DRIVER_SV
 
 class log_driver extends uvm_driver#(log_seq_item);
 	`uvm_component_utils(log_driver)
 	virtual interface axi_lite_if vif;
 	
 	
 	function new(string name = "log_driver", uvm_component parent = null);
      		super.new(name,parent);

   	endfunction
   	
    	function void connect_phase(uvm_phase phase);
        	super.connect_phase(phase);
        	if (!uvm_config_db#(virtual axi_lite_if)::get(this, "", "axi_lite_if", vif))
         	   `uvm_fatal("NOVIF",{"(DRIVER)virtual interface must be set for: ",get_full_name(),".vif"})
   	endfunction : connect_phase
 
 
 	task run_phase(uvm_phase phase);
 	@(negedge vif.rst);
 	forever begin
 	  seq_item_port.get_next_item(req);
         `uvm_info(get_type_name(),
                   $sformatf("Driver sending...\n%s", req.sprint()),
                   UVM_FULL)
             
                   
          @(posedge vif.clk)begin
          
          	 if(req.read_write)begin//read = 0, write = 1
	            vif.s00_axi_awaddr = req.addr;
	            vif.s00_axi_awvalid = 1;
	            vif.s00_axi_wdata = req.data;
	            vif.s00_axi_wvalid = 1;
	            vif.s00_axi_bready = 1'b1;	       
	            wait(vif.s00_axi_awready && vif.s00_axi_wready);	       
	            wait(vif.s00_axi_bvalid);
	            //vif.s00_axi_wdata = 0;
	            vif.s00_axi_awvalid = 0; 
	            vif.s00_axi_wvalid =  0;
               wait(!vif.s00_axi_bvalid);	   
	            vif.s00_axi_bready = 0;
	         end // if (req.read_write)
	         else begin
	            vif.s00_axi_araddr = req.addr;
                    vif.s00_axi_arvalid = 1;
                    vif.s00_axi_rready = 1;
	            wait(vif.s00_axi_arready);
                    wait(vif.s00_axi_rvalid);	           
	            vif.s00_axi_arvalid = 0;
                    vif.s00_axi_araddr = 0;
	            wait(!vif.s00_axi_rvalid);
                    req.data = vif.s00_axi_rdata;               
	            vif.s00_axi_rready = 0;	       
	         end
          
          end // @posedge
          
          seq_item_port.item_done();
 	end
 	endtask : run_phase
 endclass
 
 
 
 `endif
