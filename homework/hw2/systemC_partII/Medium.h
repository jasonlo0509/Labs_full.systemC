#ifndef MEDIUM_H_
#define MEDIUM_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#define WHITE 255
#define BLACK 0
#define MASK_X 3
#define MASK_Y 3

class Medium : public sc_module {
public:

  SC_HAS_PROCESS(Medium);

  Medium(sc_module_name n);
  ~Medium();

  sc_fifo_out< int > or_result;
  sc_fifo_out< int > og_result;
  sc_fifo_out< int > ob_result;
  sc_fifo_in< int > i_r;
  sc_fifo_in< int > i_g;
  sc_fifo_in< int > i_b;

private:
  int filterY;
  int filterX;
    //color arrays (global variable)
  int red[MASK_X * MASK_Y];
  int green[MASK_X * MASK_Y];
  int blue[MASK_X * MASK_Y];
  int red_backup[MASK_X * MASK_Y];
  int green_backup[MASK_X * MASK_Y];
  int blue_backup[MASK_X * MASK_Y];
  unsigned int rgb_raw_data_offset;
  unsigned int width;
  unsigned int height;
  unsigned int width_bytes;
  unsigned short bit_per_pixel;
  unsigned short byte_per_pixel;

  void do_medium();
  int selectKth(int* data, int s, int e, int k);
};
#endif
