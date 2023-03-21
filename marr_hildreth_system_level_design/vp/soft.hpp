#ifndef SOFT_HPP
#define SOFT_HPP

#include <iostream>
#include <fstream>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include </usr/include/opencv4/opencv2/opencv.hpp>
#include </usr/include/opencv4/opencv2/core/core.hpp>
#include </usr/include/opencv4/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv4/opencv2/highgui/highgui.hpp>


#include <tlm_utils/tlm_quantumkeeper.h>
#include <systemc>

#include "typedefs.hpp"
#include "address.hpp"
#include "DMA.hpp"
#include <vector>
using namespace std;
using namespace tlm;
using namespace sc_dt;


class Soft : public sc_core::sc_module
{

public:
	Soft(sc_core::sc_module_name name,char** strings, int argv);
	~Soft();
	
	struct PIC
	{
    unsigned int nChannel;
    bool InterLeaved;
    unsigned int Width, Height, Maxval;
    BYTE *img;
	};

	tlm_utils::simple_initiator_socket<Soft> soft_ic_i;
	tlm_utils::simple_target_socket<Soft> soft_target_dma;

protected:
	
	
	int p = 0;
	
	sc_int<11> w,h;
	
	sc_core::sc_time offset;
	void b_transport(pl_t&, sc_core::sc_time&);
	
	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void toUchar(unsigned char *buf,int val);
	int toInt(unsigned char *buf);
	void LoadImage(ifstream &infile, PIC &pic);
	bool Load5Header(ifstream &infile, PIC &pic);
	void software();
	void write_hard(sc_dt::uint64 addr,int val);
	void read_hard(sc_dt::uint64 addr, sc_uint<2> val);

	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);	
	static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);


};

#endif
