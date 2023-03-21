#ifndef INTER_CONNECT_C
#define INTER_CONNECT_C


#include "interconnect.hpp"
#include "address.hpp"

using namespace std;
using namespace tlm;
using namespace sc_core;
using namespace sc_dt;

interconnect::interconnect(sc_module_name name) : sc_module(name)
{
	ic_target.register_b_transport(this, &interconnect::b_transport);
	SC_REPORT_INFO("Interconnect", "Constructed");
}
interconnect::~interconnect()
{
  SC_REPORT_INFO("Interconnect", "Destroyed.");
}

void interconnect::b_transport(pl_t& pl, sc_core::sc_time& offset)
{
				uint64 addr = pl.get_address(); // 0x1180001
				uint64 tmp_addr;
				
				offset += sc_time(2,SC_NS);

				if(addr >= VP_IP_HARD && addr <= VP_IP_ADDR_START)
				{
							tmp_addr = addr - VP_IP_HARD;
							pl.set_address(tmp_addr);
							ic_initiator_ip -> b_transport(pl, offset);
				}

				else if (addr >= VP_DMA && addr <= VP_DMA_DESTINATION_ADDR)
				{
							tmp_addr = addr - VP_DMA;
							pl.set_address(tmp_addr);
							ic_initiator_dma -> b_transport(pl, offset);
				}

				else
				{
							SC_REPORT_ERROR("Interconnect", "Wrong address.");
							pl.set_response_status ( tlm::TLM_ADDRESS_ERROR_RESPONSE );
				}
						
				offset += sc_time(100, SC_NS);
}
#endif