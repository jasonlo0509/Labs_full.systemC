#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Medium.h"


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

Medium::Medium(sc_module_name n) : sc_module(n) {
  SC_THREAD(write_bmp);

  SC_THREAD(do_medium);

  SC_THREAD(read_bmp);
}

Medium::~Medium() {}

void Medium::read_bmp(){
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

  for(y = 0; y < height; y++) {
    for(x = 0; x < width; x++) {
      int n = 0;
      //set the color values in the arrays
      for(filterY = 0; filterY < MASK_Y; filterY++) {
        for(filterX = 0; filterX < MASK_X; filterX++) {
          int imageX = (x - MASK_X / 2 + filterX + width) % width;
          int imageY = (y - MASK_Y / 2 + filterY + height) % height;
          red[n] = *(image_s + byte_per_pixel * (imageY * width + imageX) + 2);
          green[n] = *(image_s + byte_per_pixel * (imageY * width + imageX) + 1);
          blue[n] = *(image_s + byte_per_pixel * (imageY * width + imageX) + 0);
          n++;
        }
      }
      _read_done.notify();
      wait(_medium_free);

    }
  }
  return 0;
}

void Medium::write_bmp(){
  for(;;){
    wait(_medium_done);
    *(image_t + byte_per_pixel * (width * y + x) + 2) = r_result;
    *(image_t + byte_per_pixel * (width * y + x) + 1) = g_result;
    *(image_t + byte_per_pixel * (width * y + x) + 0) = b_result;
    if(x < width-1 || y < height-1 )
      _write_free.notify();
    else
      break;
  }
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


// selects the k-th largest element from the data between start and end (end exclusive)
int Medium::selectKth(int* data, int s, int e, int k)  // in practice, use C++'s nth_element, this is for demonstration only
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
  for(;;){
    wait(_read_done);
    int filterSize = MASK_X * MASK_Y;
    r_result = red[selectKth(red, 0, filterSize, filterSize / 2)];
    g_result = green[selectKth(green, 0, filterSize, filterSize / 2)];
    b_result = blue[selectKth(blue, 0, filterSize, filterSize / 2)];
    _medium_done.notify();
    wait(_write_free);
    _medium_free.notify();
  }
}
