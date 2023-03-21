#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <string>
#include <array>
#include <vector>
//#include "address.hpp"
using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace sc_dt;

static const int DRAM_SIZE = 326526;

class memory :
	public sc_core::sc_module
{
	public:
		memory(sc_core::sc_module_name);

		tlm_utils::simple_target_socket<memory> soft_t;
		tlm_utils::simple_target_socket<memory> dma_t;
		
			
	protected:
		//static const int DRAM_SIZE = 1024;
		unsigned char dram[DRAM_SIZE];
	
		typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
		void b_transport (pl_t&, sc_core::sc_time&);

};

#endif