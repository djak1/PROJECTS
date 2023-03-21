#ifndef _VP_
#define _VP_

#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>



#include "soft.hpp"
#include "interconnect.hpp"
#include "ip_hard.hpp"
#include "DMA.hpp"
#include "address.hpp"

using namespace sc_core;
using namespace std;
using namespace sc_dt;

class Vp: public sc_core::sc_module
{
	public:
		Vp(sc_core::sc_module_name name,char** strings, int argv);
		~Vp();
	protected:
		
	DMA dma;
	Soft soft;
	IP_HARD ip_hard;
	interconnect ic;

	sc_fifo<int> f0;
	sc_fifo<int> f1;
};

#endif