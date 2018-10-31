#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "Medium.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

int sc_main(int argc, char **argv) {
	Medium medium("medium");

	sc_start();
	cout<< "Simulated time == " << sc_core::sc_time_stamp() << std::endl;

	return 0;
}