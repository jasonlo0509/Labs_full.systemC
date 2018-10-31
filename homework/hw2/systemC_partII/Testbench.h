#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#define WHITE 255
#define BLACK 0
#define MASK_X 3
#define MASK_Y 3

class Testbench : public sc_module {
public:

  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench();

  void read_bmp();
  void write_bmp();
  void input();
  void output();

  sc_fifo_out< int > o_r;
  sc_fifo_out< int > o_g;
  sc_fifo_out< int > o_b;
  sc_fifo_in< int > ir_result;
  sc_fifo_in< int > ig_result;
  sc_fifo_in< int > ib_result;

private:
  int filterY;
  int filterX;
  int R;
  int G;
  int B;
  unsigned int rgb_raw_data_offset;
  unsigned int width;
  unsigned int height;
  unsigned int width_bytes;
  unsigned short bit_per_pixel;
  unsigned short byte_per_pixel;
  unsigned char *image_s;     // source image array
  unsigned char *image_t;     // target image array

};
#endif
