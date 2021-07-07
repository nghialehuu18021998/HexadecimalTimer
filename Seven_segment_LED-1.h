#ifndef __SevenSeg__
#define __SevenSeg__
#include "NUC100Series.h"

typedef struct binary{
	unsigned short x:1;
} bin;

bin* decimal_to_binary(int decimal);

void EnableSegment(uint32_t segment_no);
void CloseSegment(uint32_t segment_no);
void CloseSevenSegment(void);
void OpenSevenSegment(void);
void ShowSevenSegment(uint8_t no, uint8_t number);
#endif
