#include "VP.hpp"
#include <iostream>


Vp::Vp(sc_core::sc_module_name name,char** strings, int argv): 
		sc_module (name), 
		soft("Soft", strings, argv), 
		ic("Interconnect"), 
		ip_hard("Ip_hard"), 
		dma("DMA"),
		f0(40000000),
		f1(40000000)
		
{	
	
	soft.soft_ic_i.bind(ic.ic_target); //T
	dma.dma_initiator_soft.bind(soft.soft_target_dma);
	ic.ic_initiator_ip.bind(ip_hard.ip_target); //T
	ic.ic_initiator_dma.bind(dma.dma_target); // T
	//povezati dma i soft
	

	dma.pfifo.bind(f0);
	ip_hard.pfifo(f0);

	ip_hard.dfifo.bind(f1);
	dma.dfifo(f1);

	SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp()
{
	SC_REPORT_INFO("Virtual Platform", "Destroyed.");
}
