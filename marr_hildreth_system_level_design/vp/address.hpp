#ifndef SOFT_HPP_ 
#define SOFT_HPP_

#include <systemc>
#include "DMA.hpp"

//static const int DRAM_SIZE = 326800;

const sc_dt::uint64 DMA_CONTROL = 0;    // Control and Status Register
const sc_dt::uint64 DMA_SOURCE_ADDR= 1;	// Source Address Register
const sc_dt::uint64 DMA_COUNT = 2;    // Count Register
const sc_dt::uint64 DMA_DESTINATION_ADDR = 3;	// Destination Address Register


const sc_dt::uint64 ADDR_HEIGHT = 1;
const sc_dt::uint64 ADDR_WIDTH = 2;
const sc_dt::uint64 ADDR_PICTURE = 3;
const sc_dt::uint64 IP_HARD_START = 4;

const sc_dt::uint64 VP_SOFT = 0x4C480000;

const sc_dt::uint64 VP_IP_HARD = 0x11880000;
const sc_dt::uint64 VP_IP_ADDR_HEIGHT = VP_IP_HARD + ADDR_HEIGHT;
const sc_dt::uint64 VP_IP_ADDR_WIDTH = VP_IP_HARD + ADDR_WIDTH;
const sc_dt::uint64 VP_IP_ADDR_START = VP_IP_HARD + IP_HARD_START;


const sc_dt::uint64 VP_IP_ADDR_PICTURE = VP_IP_HARD + ADDR_PICTURE;

const sc_dt::uint64 IMAGE_START = 0x7f1bf9dcc010;


const sc_dt::uint64 VP_DMA = 0x88110000;
const sc_dt::uint64 VP_DMA_CONTROL_ADDR = VP_DMA + DMA_CONTROL;
const sc_dt::uint64 VP_DMA_SOURCE_ADDR = VP_DMA + DMA_SOURCE_ADDR;
const sc_dt::uint64 VP_DMA_COUNT_ADDR = VP_DMA + DMA_COUNT;
const sc_dt::uint64 VP_DMA_DESTINATION_ADDR = VP_DMA + DMA_DESTINATION_ADDR;


const sc_dt::uint64 VP_MEMORY = 0x81810000;
const sc_dt::uint64 VP_MEMORY_HIGH = VP_MEMORY + 408170;



#endif