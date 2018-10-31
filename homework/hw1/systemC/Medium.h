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

  void read_bmp();
  void write_bmp();

private:
  int filterY;
  int filterX;
  int x;
  int y;
  int r_result;
  int g_result;
  int b_result;
  //color arrays (global variable)
  int red[MASK_X * MASK_Y];
  int green[MASK_X * MASK_Y];
  int blue[MASK_X * MASK_Y];
  unsigned int rgb_raw_data_offset;
  unsigned int width;
  unsigned int height;
  unsigned int width_bytes;
  unsigned short bit_per_pixel;
  unsigned short byte_per_pixel;
  unsigned char *image_s;     // source image array
  unsigned char *image_t;     // target image array

  void do_medium();
  int selectKth(int* data, int s, int e, int k);

  sc_event _read_done;
  sc_event _medium_done;
  sc_event _write_free;
  sc_event _medium_free;
};
#endif
