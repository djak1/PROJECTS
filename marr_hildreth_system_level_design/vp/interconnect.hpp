#ifndef _INTER_CONNECT_
#define _INTER_CONNECT_

#include <systemc>
#include <tlm>
#include "typedefs.hpp"
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

class interconnect : public sc_core::sc_module
{
public:
	interconnect(sc_core::sc_module_name name);
	~interconnect();
	tlm_utils::simple_target_socket<interconnect> ic_target;
	tlm_utils::simple_initiator_socket<interconnect> ic_initiator_ip;
	tlm_utils::simple_initiator_socket<interconnect> ic_initiator_dma;

protected:
	void b_transport(pl_t&, sc_core::sc_time&);

	void msg(const pl_t&);

};
#endif
