`ifndef LOG_BASE_FULL_SEQ_SV
 `define LOG_BASE_FULL_SEQ_SV

class base_full_seq extends uvm_sequence#(log_seq_item_full);

   `uvm_object_utils(base_full_seq)
   `uvm_declare_p_sequencer(log_sequencer_full)
   
   

   function new(string name = "base_full_seq");
      super.new(name);
   endfunction

   // objections are raised in pre_body
   virtual task pre_body();
      uvm_phase phase = get_starting_phase();
      if (phase != null)
        phase.raise_objection(this, {"Running sequence '", get_full_name(), "'"});
   endtask : pre_body

   // objections are dropped in post_body
   virtual task post_body();
      uvm_phase phase = get_starting_phase();
      if (phase != null)
        phase.drop_objection(this, {"Completed sequence '", get_full_name(), "'"});
   endtask : post_body

endclass : base_full_seq

`endif
















