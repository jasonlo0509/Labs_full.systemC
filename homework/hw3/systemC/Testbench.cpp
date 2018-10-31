#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"


unsigned char header[54] = {
    0x42,          // identity : B
    0x4d,          // identity : M
    0,    0, 0, 0, // file size
    0,    0,       // reserved1
    0,    0,       // reserved2
    54,   0, 0, 0, // RGB data offset
    40,   0, 0, 0, // struct BITMAPINFOHEADER size
    0,    0, 0, 0, // bmp width
    0,    0, 0, 0, // bmp height
    1,    0,       // planes
    24,   0,       // bit per pixel
    0,    0, 0, 0, // compression
    0,    0, 0, 0, // data size
    0,    0, 0, 0, // h resolution
    0,    0, 0, 0, // v resolution
    0,    0, 0, 0, // used colors
    0,    0, 0, 0  // important colors
};

Testbench::Testbench(sc_module_name n) : sc_module(n), initiator("initiator") {
  SC_THREAD(inout);
}

Testbench::~Testbench() {}


void Testbench::inout(){
  unsigned char mask[4];
  word data;
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      //set the color values in the arrays
      if(x == 0){
        for(filterY = 0; filterY < MASK_Y; filterY++) {
          for(filterX = 0; filterX < MASK_X; filterX++) {
            int imageX = (x - MASK_X / 2 + filterX + width) % width;
            int imageY = (y - MASK_Y / 2 + filterY + height) % height;
            R = *(image_s + byte_per_pixel * (imageY * width + imageX) + 2);
            G = *(image_s + byte_per_pixel * (imageY * width + imageX) + 1);
            B = *(image_s + byte_per_pixel * (imageY * width + imageX) + 0);
            data.uc[0] = R;
            data.uc[1] = G;
            data.uc[2] = B;
            mask[0] = 0xff;
            mask[1] = 0xff;
            mask[2] = 0xff;
            mask[3] = 0;
            initiator.write_to_socket( SOBEL_FILTER_R_ADDR, mask , data.uc, 4 );
          }
        }
      }
      else {
        for(filterY = 0; filterY < MASK_Y; filterY++) {
          filterX = MASK_X - 1;
          int imageX = (x - MASK_X / 2 + filterX + width) % width;
          int imageY = (y - MASK_Y / 2 + filterY + height) % height;
          R = *(image_s + byte_per_pixel * (imageY * width + imageX) + 2);
          G = *(image_s + byte_per_pixel * (imageY * width + imageX) + 1);
          B = *(image_s + byte_per_pixel * (imageY * width + imageX) + 0);
          data.uc[0] = R;
          data.uc[1] = G;
          data.uc[2] = B;
          mask[0] = 0xff;
          mask[1] = 0xff;
          mask[2] = 0xff;
          mask[3] = 0;
          initiator.write_to_socket( SOBEL_FILTER_R_ADDR, mask , data.uc, 4 );
        }
      }
      
      initiator.read_from_socket( SOBEL_FILTER_RESULT_ADDR, mask, data.uc, 4);
      *(image_t + byte_per_pixel * (width * y + x) + 2) = data.uc[0];
      *(image_t + byte_per_pixel * (width * y + x) + 1) = data.uc[1];
      *(image_t + byte_per_pixel * (width * y + x) + 0) = data.uc[2];
    }
  }
  sc_stop();
}


void Testbench::read_bmp(){
  const char *fname_s = "lena_noise.bmp";
  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(fname_s, "rb");
  if (fp_s == NULL) {
    printf("fopen fp_s error\n");
    return -1;
  }

  // move offset to 10 to find rgb raw data offset
  fseek(fp_s, 10, SEEK_SET);
  fread(&rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s);
 
  // move offset to 18 to get width & height;
  fseek(fp_s, 18, SEEK_SET);
  fread(&width,  sizeof(unsigned int), 1, fp_s);
  fread(&height, sizeof(unsigned int), 1, fp_s);
 
  // get bit per pixel
  fseek(fp_s, 28, SEEK_SET);
  fread(&bit_per_pixel, sizeof(unsigned short), 1, fp_s);
  byte_per_pixel = bit_per_pixel / 8;
 
  // move offset to rgb_raw_data_offset to get RGB raw data
  fseek(fp_s, rgb_raw_data_offset, SEEK_SET);
     
  image_s = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
  if (image_s == NULL) {
    printf("malloc images_s error\n");
    return -1;
  }
   
  image_t = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
  if (image_t == NULL) {
    printf("malloc image_t error\n");
    return -1;
  }
   
  fread(image_s, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_s);
  fclose(fp_s);

  return 0;
}

void Testbench::write_bmp(){
  
  const char *fname_t = "lena_medium.bmp";
  FILE *fp_t = NULL;      // target file handler
  unsigned int file_size; // file size
 
  fp_t = fopen(fname_t, "wb");
  if (fp_t == NULL) {
    printf("fopen fname_t error\n");
    return -1;
  }
      
  // file size 
  file_size = width * height * byte_per_pixel + rgb_raw_data_offset;
  header[2] = (unsigned char)(file_size & 0x000000ff);
  header[3] = (file_size >> 8)  & 0x000000ff;
  header[4] = (file_size >> 16) & 0x000000ff;
  header[5] = (file_size >> 24) & 0x000000ff;
    
  // width
  header[18] = width & 0x000000ff;
  header[19] = (width >> 8)  & 0x000000ff;
  header[20] = (width >> 16) & 0x000000ff;
  header[21] = (width >> 24) & 0x000000ff;
    
  // height
  header[22] = height &0x000000ff;
  header[23] = (height >> 8)  & 0x000000ff;
  header[24] = (height >> 16) & 0x000000ff;
  header[25] = (height >> 24) & 0x000000ff;
    
  // bit per pixel
  header[28] = bit_per_pixel;
  
  // write header
  fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_t);
 
  // write image
  fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_t);

  fclose(fp_t);
}


