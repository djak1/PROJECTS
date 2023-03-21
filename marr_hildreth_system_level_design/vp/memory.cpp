#include "memory.hpp"



memory::memory(sc_module_name name): sc_module(name), soft_t("soft_t"),dma_t("dma_t")
{
	soft_t.register_b_transport(this, &memory::b_transport);
	dma_t.register_b_transport(this, &memory::b_transport);

	for (int i = 0; i < DRAM_SIZE; ++i)
	{
		dram[i] = 0;
	}

}

void memory::b_transport(pl_t& pl, sc_core::sc_time& offset)
{
	
	tlm_command cmd    = pl.get_command(); 
	uint64 adr         = pl.get_address(); 
	unsigned char *buf = pl.get_data_ptr();
	unsigned int len   = pl.get_data_length();
	cout << " Adresa "  << adr << endl;
	cout << " Duzina podataka "  << len << endl;


	switch(cmd)
    {
    case TLM_WRITE_COMMAND:


      for (unsigned int i = 0; i <= len; ++i)
        {
          dram[adr++] = buf[i];
         }
        cout << "UPISALO SE U MEMORIJU" << endl;
        
      pl.set_response_status( TLM_OK_RESPONSE );
      break;


    case TLM_READ_COMMAND:
      cout << "PORUKA " << endl;
      for (unsigned int i = 0; i <= len; ++i)
        {
        //cout << "PORUKA " << i << " Adresa = " << adr <<endl;
          buf[i] = dram[adr++];
         // cout << " Duzina je " << len << endl;
        }
      pl.set_response_status( TLM_OK_RESPONSE );
      break;

    default:
      pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
    }

  offset += sc_time(10, SC_NS);
  cout << "**********MEMORY**********" << endl;
 
}



