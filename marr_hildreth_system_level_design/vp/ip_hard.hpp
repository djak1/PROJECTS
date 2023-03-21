#ifndef _SYS_BUS_HPP_
#define _SYS_BUS_HPP_

#include <systemc>
#include <tlm>
#include <vector>
#include "typedefs.hpp"
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>




using namespace sc_core;
using namespace sc_dt;
using namespace std;

class IP_HARD : public sc_core::sc_module
{
public:
	IP_HARD(sc_core::sc_module_name name);
	~IP_HARD();

	tlm_utils::simple_target_socket<IP_HARD> ip_target;
	
	sc_fifo_in<int> pfifo;
	sc_port<sc_fifo_out_if<int>> dfifo; 
	sc_core::sc_event s0;
	

protected:
	pl_t pl;
	sc_core::sc_time offset;
	unsigned char** matrix;
	int** newinp;
	unsigned char** im;

	sc_uint<11> width;
	sc_uint<11> height;
	sc_uint<11> picture;
	sc_dt::sc_uint<2> start;
	sc_dt::sc_uint<2> stop;
	
	std::vector<int> read_pic;
	int logmask[5][5]={ 	{0, 0, -1, 0, 0},
                            {0, -1, -2, -1, 0},
                            {-1, -2, 16, -2, -1},
                            {0, -1, -2, -1, 0},
                            {0, 0, -1, 0, 0} };
	

	void b_transport(pl_t&, sc_core::sc_time&);
	void process();
	void createPicture(vector<int> vect);
	void LaplacianHW(int** newinp, int height, int width, unsigned char** matrix);


	int toInt(unsigned char *buf);

	sc_logic toggle;
	std::vector<sc_int<11>> tmp_mem;
	vector<sc_int<11>>::iterator it;

};
#endif