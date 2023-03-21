#ifndef DMA_C
#define DMA_C
#include "DMA.hpp"
#include "typedefs.hpp"
#include <tlm_utils/tlm_quantumkeeper.h>
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

DMA::DMA(sc_module_name name) : sc_module(name)
{
   dma_target.register_b_transport(this, &DMA::b_transport);
   SC_METHOD(send);
}

void DMA::b_transport(pl_t &pl, sc_time &offset)
{
   tlm_command cmd = pl.get_command(); //read
   uint64 addr = pl.get_address(); //VP_DMA
   unsigned char *data = pl.get_data_ptr(); // picture_laplacian
   unsigned int len = pl.get_data_length(); // 4

   switch(cmd)
   {
      case TLM_WRITE_COMMAND:
         switch(addr)
         {
            case DMA_CONTROL:
               control = *((sc_uint<4>*)data);
               cout << "DMA: Control bit " << control << endl << endl;
               send();
               offset += sc_time(1000, SC_NS);
               pl.set_response_status(TLM_OK_RESPONSE);
            break;

            case DMA_COUNT:
               
               count = *((sc_uint<32>*)data);
               cout<<"DMA: Maximum data size : "<< count <<endl <<endl;
               pl.set_response_status(TLM_OK_RESPONSE);
               break;

            case DMA_DESTINATION_ADDR:
               destination_addr = *((sc_uint<32>*)data);
               cout << "DMA: Destination address: " << destination_addr << endl << endl;
               pl.set_response_status(TLM_OK_RESPONSE);
            break;


         }
      break;

      case TLM_READ_COMMAND:
         switch(addr)
         {
            case VP_DMA: 
            
            data = read_pic.data();
          
            pl.set_response_status(TLM_OK_RESPONSE);
            break;

         }

      break;

      default:
          pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
      break;

   offset += sc_time(10, SC_NS);
   }

}

void DMA::send()
{ 
   cout << "****Welcome to SEND() function****" << endl;
         tlm_generic_payload pl;
         tlm_utils::tlm_quantumkeeper qk;
         qk.reset();
         
         sc_time off = SC_ZERO_TIME;

         unsigned char* buffer;
         unsigned char* picture_for_soft;
         picture_for_soft = new unsigned char[count];
         buffer = new unsigned char[count];
      
         if (destination_addr == VP_IP_HARD)
         {
            pl.set_command(TLM_READ_COMMAND);
            pl.set_address(VP_SOFT);
            pl.set_data_ptr((unsigned char*)buffer);
            pl.set_data_length(2);

            //qk.inc(sc_time(40, SC_NS));
                // offset = qk.get_local_time();
           
            dma_initiator_soft->b_transport(pl, offset);

              for (int i = 0; i < count; ++i)
              {
                  pfifo ->write(buffer[i]);
              }

         }
         else if (destination_addr == VP_MEMORY)
         {
            
            int v;
            for(int i = 0; i < count; i++)
            {
               v = dfifo -> read();
               picture_for_soft[i] = v;
         
            }
         
          pl.set_command(TLM_WRITE_COMMAND);
          pl.set_address(VP_SOFT);
          pl.set_data_ptr((unsigned char*)picture_for_soft);
          pl.set_data_length(2);

          //qk.inc(sc_time(40, SC_NS));
            //  offset = qk.get_local_time();
          dma_initiator_soft->b_transport(pl, offset);

          

         }

}

#endif