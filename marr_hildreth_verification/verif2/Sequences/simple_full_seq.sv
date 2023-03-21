`ifndef LOG_SIMPLE_SEQ_FULL_SV
 `define LOG_SIMPLE_SEQ_FULL_SV
 
//Axi Lite sequence
class log_simple_seq_full extends base_full_seq;
   int num_of_images = 0;
   int image = 0; 
   
 `uvm_object_utils (log_simple_seq_full)
   function new(string name = "log_simple_seq_full");
      super.new(name);
   endfunction
   
   
   virtual task body();
   
   	int kernel = 0;
   	int pixel_value = 0;
   	int count = 0;
   	bit [7:0] data_s;
   	
   	
      `uvm_do_with(req, {req.read_write == 0; req.length == 8'hFF; req.addr1 == 12'h0;});
      `uvm_do_with(req, {req.read_write == 0; req.length == 8'hFF; req.addr1 == 12'h200;});
      
      `uvm_do_with(req, {req.read_write == 0; req.length == 8'h19; req.addr1 == 12'h400;});
      
   //   `uvm_do_with(req, {req.read_write == 1; req.length == 8'h64; req.addr1 == 12'h0;});
     // `uvm_do_with(req, {req.read_write == 1; req.length == 8'h19; req.addr1 == 12'h400;});

     
    endtask : body 
   
endclass

`endif






















