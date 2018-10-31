#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

#define MASK_N 2
#define MASK_X 3
#define MASK_Y 3

#define SOBEL_FILTER_R_ADDR (0x00000000)

#define SOBEL_FILTER_RESULT_ADDR (0x00000004)

typedef union word{
	unsigned char uc[4];
} word;

#endif
