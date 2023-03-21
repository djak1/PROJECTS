`ifndef LOG_VIRTUAL_SEQ_SV
 `define LOG_VIRTUAL_SEQ_SV

class virtual_seq extends base_lite_seq;

    `uvm_object_utils (virtual_seq)

    uvm_sequencer #(log_seq_item) sequencer_lite_if;
        
    function new(string name = "virtual_seq");
	   super.new(name);
    endfunction

    virtual task body();

	   log_simple_seq sequence_lite = log_simple_seq::type_id::create("interface1_seq_lite"); 
	   sequence_lite.start(sequencer_lite_if);

    endtask : body

endclass : virtual_seq

class virtual_seq1 extends base_full_seq;

    `uvm_object_utils (virtual_seq1)

    uvm_sequencer #(log_seq_item_full) sequencer_full_if;
        
    function new(string name = "virtual_seq1");
	   super.new(name);
    endfunction

    virtual task body();

	   log_simple_seq_full sequence_full = log_simple_seq_full::type_id::create("interface1_seq_full"); 
	   sequence_full.start(sequencer_full_if);

    endtask : body

endclass : virtual_seq1
//*************************************************//
class virtual_seq2 extends base_lite_seq;

    `uvm_object_utils (virtual_seq2)

    uvm_sequencer #(log_seq_item) sequencer_lite1_if;
        
    function new(string name = "virtual_seq2");
	   super.new(name);
    endfunction

    virtual task body();

	   seq_start_laplas sequence_lite1 = seq_start_laplas::type_id::create("interface1_seq_lite2"); 
	   sequence_lite1.start(sequencer_lite1_if);

    endtask : body

endclass : virtual_seq2



`endif
