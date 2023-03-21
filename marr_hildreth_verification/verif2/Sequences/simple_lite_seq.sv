`ifndef LOG_SIMPLE_LITE_SEQ_SV
 `define LOG_SIMPLE_LITE_SEQ_SV
 
//Axi Lite sequence
class log_simple_seq extends base_lite_seq;
   int num_of_images = 0;
   int image = 0; 
   
 `uvm_object_utils (log_simple_seq)
   function new(string name = "log_simple_seq");
      super.new(name);
   endfunction
   
   
   virtual task body();
      
       
       `uvm_do_with(req, {req.read_write == 1; req.data == 21; req.addr == 1;});
       `uvm_do_with(req, {req.read_write == 1; req.data == 24; req.addr == 4;});
       `uvm_do_with(req, {req.read_write == 1; req.data == 3; req.addr == 8;});
       `uvm_do_with(req, {req.read_write == 1; req.data == 5; req.addr == 12;});
       //`uvm_do_with(req, {req.read_write == 1; req.data == 1; req.addr == 16;});
   //    `uvm_do_with(req, {req.read_write == 1; req.data == 1; req.addr == 20;});
      // `uvm_do_with(req, {req.read_write == 1; req.data == 1; req.addr == 24;});
       
       
      // `uvm_do_with(req, {req.read_write == 0; req.addr == 0;});
      // `uvm_do_with(req, {req.read_write == 0; req.addr == 4;});
      // `uvm_do_with(req, {req.read_write == 0; req.addr == 8;});
       //`uvm_do_with(req, {req.read_write == 0; req.addr == 12;});
      // `uvm_do_with(req, {req.read_write == 0; req.addr == 16;});
     //  `uvm_do_with(req, {req.read_write == 0; req.addr == 20;});
       

    endtask : body 
   
endclass

class seq_start_laplas extends base_lite_seq;
   
   
 `uvm_object_utils (seq_start_laplas)
   function new(string name = "seq_start_laplas");
      super.new(name);
   endfunction
   
   
    virtual task body();

       
       for(int i = 0; i < 5; i++)begin
       	`uvm_do_with(req, {req.read_write == 1; req.data == 1; req.addr == 16;});
       end
       `uvm_do_with(req, {req.read_write == 1;  req.data == 0; req.addr == 16;});
        
       forever begin  
                `uvm_do_with(req, {req.read_write == 0; req.addr == 24;});        
                 if (req.data == 1) break;                                  
        end
         
         for(int i = 0; i < 5; i++)begin
            `uvm_do_with(req, {req.read_write == 1; req.data == 1; req.addr == 20;});
         end
        `uvm_do_with(req, {req.read_write == 1; req.data == 0; req.addr == 20;});
        
      
        forever begin
                `uvm_do_with(req, {req.read_write == 0; req.addr == 24;});        
                 if (req.data == 1) break;                                  
        end
  
    endtask : body  
   
endclass



`endif






















