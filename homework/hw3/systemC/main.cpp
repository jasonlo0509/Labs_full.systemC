#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "Testbench.h"
#include "Medium.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

int sc_main(int argc, char **argv) {
	Testbench tb("tb");
	Medium medium("medium");
	tb.initiator.i_skt( medium.t_skt );

	tb.read_bmp();
	sc_start();
	cout<< "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
	tb.write_bmp();

	return 0;
}