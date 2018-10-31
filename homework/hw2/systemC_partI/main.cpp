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
	sc_fifo<int> r;
	sc_fifo<int> g;
	sc_fifo<int> b;
	sc_fifo<int> r_result;
	sc_fifo<int> g_result;
	sc_fifo<int> b_result;

	tb.o_r(r);
	tb.o_g(g);
	tb.o_b(b);
	tb.ir_result(r_result);
	tb.ig_result(g_result);
	tb.ib_result(b_result);


	medium.i_r(r);
	medium.i_g(g);
	medium.i_b(b);
	medium.or_result(r_result);
	medium.og_result(g_result);
	medium.ob_result(b_result);


	tb.read_bmp();
	sc_start();
	cout<< "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
	tb.write_bmp();

	return 0;
}