`uvm_analysis_imp_decl(_lite)
`uvm_analysis_imp_decl(_full)
class scoreboard extends uvm_scoreboard;

   // control fileds
   bit checks_enable = 1;
   bit coverage_enable = 1;
   bit checks_enable1 = 1;
   bit coverage_enable1 = 1;
   
      
   int height;
   int width;
   int l1;
   int border1;
   int spix, maxi, th;
   int zandar;
      int dama;

    logic [15:0] matrix_picture[$][$];
    logic [15:0] matrix_mask[$][$];
    logic [15:0] matrix_jump[$][$];
   logic [15:0]  result_matrix[$][$];
   
   
   logic[15:0] picture[$];
   logic [15:0] mask[$];
   
   
   // This TLM port is used to connect the scoreboard to the monitor
  uvm_analysis_imp_lite#(log_seq_item, scoreboard) port_axi_lite;
  uvm_analysis_imp_full#(log_seq_item_full, scoreboard) port_axi_full;
   
   int num_of_tr = 0;

   `uvm_component_utils_begin(scoreboard)
      `uvm_field_int(checks_enable, UVM_DEFAULT)
      `uvm_field_int(coverage_enable, UVM_DEFAULT)
      `uvm_field_int(checks_enable1, UVM_DEFAULT)
      `uvm_field_int(coverage_enable1, UVM_DEFAULT)
   `uvm_component_utils_end

   function new(string name = "scoreboard", uvm_component parent = null);
      super.new(name,parent);
      port_axi_lite = new("port_axi_lite", this);
      port_axi_full = new("port_axi_full", this);
   endfunction : new
   
   function void report_phase(uvm_phase phase);
      `uvm_info(get_type_name(), $sformatf("Calc scoreboard examined: %0d transactions", num_of_tr), UVM_LOW);
   endfunction : report_phase
   

   function void write_lite (log_seq_item lite_item);
    log_seq_item item_clone;
    if(checks_enable)begin
       
        if(lite_item.addr == 0)begin
        	width = lite_item.data;
        	$display("SCOREBOARD : Width := %d \n", width);
        end
        else if(lite_item.addr == 4)begin
        	height = lite_item.data;
        	$display("SCOREBOARD : Height := %d \n", height);
        end
        else if (lite_item.addr == 8)begin
        	border1 = lite_item.data;
        	$display("SCOREBOARD : Border1 := %d \n", border1);
        end
        else if (lite_item.addr == 12)begin
        	l1 = lite_item.data;
        	$display("SCOREBOARD : L1 := %d \n", l1);
        end
        //`uvm_info(get_type_name(), $sformatf("Axi Lite DONE transaction"), UVM_LOW);       
    end
   endfunction : write_lite
   
   
   //AXI FULL-************************************************************************************************* 
   function void write_full (log_seq_item_full full_item);
     // log_seq_item_full item;
     // $cast(item, full_item.clone());
        
      if(checks_enable1)begin 
          //item.data1 = full_item.data1;
         if(full_item.addr1 == 0)begin
       	       picture.push_back(full_item.data1); 
	       $display(" SCOREBOARD LOADING PICTURE");
	 end
	 else if (full_item.addr1 == 1024) begin
	       mask.push_back(full_item.data1); 
	       $display(" SCOREBOARD LOADING MASK");
	 end

	 if(mask.size() == full_item.length)begin
	    laplacian(picture, mask, height, width, l1, border1);
	 end
      end // if (checks_enable1)
      
   endfunction : write_full

   function void laplacian (logic [15:0] picture[$], logic [15:0] mask[$], int height, int width, int l1, int border1);
      $display("Laplacian START");
      zandar = 0;
      dama = 0;
      
      maxi = 0;
      th = 0;
      
      for(int i = 0; i < height; i++)begin
	 for (int j = 0; j < width; j++)begin
	    
	    matrix_picture[i][j] = picture[zandar];
	    zandar++;
  
	 end
      end

      for(int i = 0; i < 5; i++)begin
	 for (int j = 0; j < 5; j++)begin
	    
	    matrix_mask[i][j] = picture[dama];
	    dama++;
  
	 end
     end
      
      for (int we = 0; we < height; we++)begin
	 for (int just = 0; just < width; just++)begin
	    spix = 0;
 
	    for (int have = 0; have < l1; have++)begin
	       for (int fun = 0; fun < l1; fun++)begin
		  spix = spix+(matrix_mask[have][fun]*(matrix_picture[we-border1+have][just-border1+fun]));
	       end
	    end
	    matrix_jump[we][just] = spix;
	    if(matrix_jump[we][just] > maxi)begin
	       maxi = matrix_jump[we][just];
	    end
	    
	 end
      end // for (int we = 0; we < height; we++)

      th = 0.75*maxi;
      //Zerro crossing and tresholding
      for (int i = 2; i < height - 2; i++)begin
	 for (int j = 2; j < width-2; j++)begin

	    if (matrix_jump[i][j] != 0)begin
	       
	       if((matrix_jump[i][j+1] >= 0 && matrix_jump[i][j-1] < 0) || (matrix_jump[i][j+1] < 0 && matrix_jump[i][j-1] >=0))begin
		  if (matrix_jump[i][j] >= th)begin
		     
		     result_matrix[i][j] = 255;
    
		  end
	       end
	       else if((matrix_jump[i+1][j] >= 0 && matrix_jump[i-1][j] < 0) || (matrix_jump[i+1][j] < 0 && matrix_jump[i-1][j] >=0))begin
		  if (matrix_jump[i][j] >= th)begin
		     
		     result_matrix[i][j] = 255;
    
		  end
	       end
	       else if((matrix_jump[i+1][j+1] >= 0 && matrix_jump[i-1][j-1] < 0) || (matrix_jump[i+1][j+1] < 0 && matrix_jump[i-1][j-1] >=0))begin
		  if (matrix_jump[i][j] >= th)begin
		     
		     result_matrix[i][j] = 255;
    
		  end
	       end
	       else if((matrix_jump[i-1][j+1] >= 0 && matrix_jump[i+1][j-1] < 0) || (matrix_jump[i-1][j+1] < 0 && matrix_jump[i+1][j-1] >=0))begin
		  if (matrix_jump[i][j] >= th)begin
		     
		     result_matrix[i][j] = 255;
    
		  end
	       end
	    end
	 end
      end

      //`uvm_info(get_type_name(), "SCOREBOARD DONE !!", UVM_HIGH)
      $display("Laplacian DONE");
      

      
 

      
   endfunction // laplacian
endclass : scoreboard

























