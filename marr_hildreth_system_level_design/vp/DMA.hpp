#ifndef DMA_HPP_ 
#define DMA_HPP_

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <string>
#include <array>
#include <vector>

#include "address.hpp"
#include "typedefs.hpp"

using namespace sc_dt;
using namespace sc_core;
using namespace std;

#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))


class DMA : public sc_core::sc_module
{
	public:
		SC_HAS_PROCESS(DMA);
		DMA(sc_core::sc_module_name name);

    	tlm_utils::simple_target_socket<DMA> dma_target;
    	tlm_utils::simple_initiator_socket<DMA> dma_initiator_soft;
    	unsigned char* buffer;

    	sc_port<sc_fifo_out_if<int>> pfifo;
    	sc_fifo_in<int> dfifo;

protected:
		
		sc_core::sc_time offset;

		void send();
		void b_transport(pl_t&, sc_core::sc_time&);

		sc_uint<2>   control;
  		sc_uint<32> val;
  		sc_uint<32>  count;
		sc_uint<32>  destination_addr;
		std::vector<unsigned char> read_pic;
		

};
#endif