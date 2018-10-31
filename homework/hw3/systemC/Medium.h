#ifndef MEDIUM_H_
#define MEDIUM_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include "tlm"
#include "tlm_utils/simple_target_socket.h"

#include "filter_def.h"

#define WHITE 255
#define BLACK 0
#define MASK_X 3
#define MASK_Y 3

class Medium : public sc_module {
public:
  tlm_utils::simple_target_socket<Medium> t_skt;

  SC_HAS_PROCESS(Medium);

  Medium(sc_module_name n);
  ~Medium();

  sc_fifo< unsigned char > or_result;
  sc_fifo< unsigned char > og_result;
  sc_fifo< unsigned char > ob_result;
  sc_fifo< unsigned char > i_r;
  sc_fifo< unsigned char > i_g;
  sc_fifo< unsigned char > i_b;

private:
  int filterY;
  int filterX;
  //color arrays (global variable)
  unsigned char red[MASK_X * MASK_Y];
  unsigned char green[MASK_X * MASK_Y];
  unsigned char blue[MASK_X * MASK_Y];
  unsigned char red_backup[MASK_X * MASK_Y];
  unsigned char green_backup[MASK_X * MASK_Y];
  unsigned char blue_backup[MASK_X * MASK_Y];
  unsigned int rgb_raw_data_offset;
  unsigned int width;
  unsigned int height;
  unsigned int width_bytes;
  unsigned short bit_per_pixel;
  unsigned short byte_per_pixel;
  unsigned int base_offset;

  void do_medium();
  unsigned char selectKth(unsigned char* data, int s, int e, int k);
  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif
