#include <string>
#include <systemc>

#include "VP.hpp"
#include "address.hpp"
#include "typedefs.hpp"

using namespace sc_core;



int sc_main(int argc, char* argv[])
{
    Vp vp("VirtualPlatform" ,argv, argc);

    tlm_global_quantum::instance().set(sc_time(10,SC_NS));
    sc_start(50000, SC_NS);


  return 0;
}
