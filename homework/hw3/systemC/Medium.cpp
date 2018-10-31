#include <cstdio>
#include <cstdlib>
#include <iomanip>

#include "Medium.h"



Medium::Medium(sc_module_name n) : sc_module(n), t_skt("t_skt"), base_offset(0) {

  SC_THREAD(do_medium);

  t_skt.register_b_transport(this, &Medium::blocking_transport);
}

Medium::~Medium() {}


// selects the k-th largest element from the data between start and end (end exclusive)
unsigned char Medium::selectKth(unsigned char* data, int s, int e, int k)  // in practice, use C++'s nth_element, this is for demonstration only
{
  // 5 or less elements: do a small insertion sort
  if(e - s <= 5)
  {
    for(int i = s + 1; i < e; i++)
      for(int j = i; j > 0 && data[j - 1] > data[j]; j--) std::swap(data[j], data[j - 1]);
    return s + k;
  }

  int p = (s + e) / 2; // choose simply center element as pivot

  // partition around pivot into smaller and larger elements
  std::swap(data[p], data[e - 1]); // temporarily move pivot to the end
  int j = s;  // new pivot location to be calculated
  for(int i = s; i + 1 < e; i++)
    if(data[i] < data[e - 1]) std::swap(data[i], data[j++]);
  std::swap(data[j], data[e - 1]);

  // recurse into the applicable partition
  if(k == j - s) return s + k;
  else if(k < j - s) return selectKth(data, s, j, k);
  else return selectKth(data, j + 1, e, k - j + s - 1); // subtract amount of smaller elements from k
}

void Medium::do_medium() {
  for(int y = 0; y < 512; y++) {
    for(int x = 0; x < 512; x++) {
      if (x == 0){
        for (unsigned int v = 0; v<MASK_Y; ++v) {
          for (unsigned int u = 0; u<MASK_X; ++u) {
            red_backup[v * MASK_X + u] = i_r.read();
            green_backup[v * MASK_X + u] = i_g.read(); 
            blue_backup[v * MASK_X + u] = i_b.read();
          }
        }
      }
      else {
        for (unsigned int v = 0; v<MASK_Y; ++v) {
          red_backup[v * MASK_X + 0] = red_backup[v * MASK_X + 1];
          red_backup[v * MASK_X + 1] = red_backup[v * MASK_X + 2];
          red_backup[v * MASK_X + 2] = i_r.read();
          green_backup[v * MASK_X + 0] = green_backup[v * MASK_X + 1];
          green_backup[v * MASK_X + 1] = green_backup[v * MASK_X + 2];
          green_backup[v * MASK_X + 2] = i_g.read();
          blue_backup[v * MASK_X + 0] = blue_backup[v * MASK_X + 1];
          blue_backup[v * MASK_X + 1] = blue_backup[v * MASK_X + 2];
          blue_backup[v * MASK_X + 2] = i_b.read();
        }
      }

      for (unsigned int v = 0; v<MASK_Y; ++v) {
        for (unsigned int u = 0; u<MASK_X; ++u) {
          red[v * MASK_X + u] = red_backup[v * MASK_X + u];
          green[v * MASK_X + u] = green_backup[v * MASK_X + u]; 
          blue[v * MASK_X + u] = blue_backup[v * MASK_X + u];
        }
      }

      int filterSize = MASK_X * MASK_Y;
      unsigned char r_result = red[selectKth(red, 0, filterSize, filterSize / 2)];
      unsigned char g_result = green[selectKth(green, 0, filterSize, filterSize / 2)];
      unsigned char b_result = blue[selectKth(blue, 0, filterSize, filterSize / 2)];
      or_result.write(r_result);
      og_result.write(g_result);
      ob_result.write(b_result);
    }
  }
}

void Medium::blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
  sc_dt::uint64 addr    = payload.get_address();
  addr = addr - base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch( payload.get_command() )
  {
    case tlm::TLM_READ_COMMAND:
      switch( addr )
      {
        case SOBEL_FILTER_RESULT_ADDR: 
          buffer.result[0] = or_result.read();
          buffer.result[1] = og_result.read();
          buffer.result[2] = ob_result.read();
          break;
        default:
          std::cerr<<"Error! Medium::blocking_transport: address 0x"
            <<std::setfill('0')<<std::setw(8)<<std::hex<<addr<<std::dec
            <<" is not valid"<<std::endl;
          break;
      }
      data_ptr[0] = buffer.result[0];
      data_ptr[1] = buffer.result[1];
      data_ptr[2] = buffer.result[2];
      break;

    case tlm::TLM_WRITE_COMMAND:
      switch( addr )
      {
        case SOBEL_FILTER_R_ADDR: 
          if ( mask_ptr[0] == 0xff ) {
            i_r.write( data_ptr[0] );
          }
          if ( mask_ptr[1] == 0xff ) {
            i_g.write( data_ptr[1] );
          }
          if ( mask_ptr[2] == 0xff ) {
            i_b.write( data_ptr[2] );
          }
          break;
        default:
          std::cerr<<"Error! Medium::blocking_transport: address 0x"
            <<std::setfill('0')<<std::setw(8)<<std::hex<<addr<<std::dec
            <<" is not valid"<<std::endl;
          break;
      }
      break;

    case tlm::TLM_IGNORE_COMMAND:
      payload.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );
      return;
    default:
      payload.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );
      return;
  }
  payload.set_response_status( tlm::TLM_OK_RESPONSE );  // Always OK
}
