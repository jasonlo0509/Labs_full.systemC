#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Medium.h"



Medium::Medium(sc_module_name n) : sc_module(n) {

  SC_THREAD(do_medium);

}

Medium::~Medium() {}


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
  while(1){
    for (unsigned int v = 0; v<MASK_Y; ++v) {
      for (unsigned int u = 0; u<MASK_X; ++u) {
        red[v * MASK_X + u] = i_r.read();
        green[v * MASK_X + u] = i_g.read(); 
        blue[v * MASK_X + u] = i_b.read();
      }
    }

    int filterSize = MASK_X * MASK_Y;
    int r_result = red[selectKth(red, 0, filterSize, filterSize / 2)];
    int g_result = green[selectKth(green, 0, filterSize, filterSize / 2)];
    int b_result = blue[selectKth(blue, 0, filterSize, filterSize / 2)];
    or_result.write(r_result);
    og_result.write(g_result);
    ob_result.write(b_result);
  }
}
