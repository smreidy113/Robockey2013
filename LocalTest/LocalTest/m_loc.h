#ifndef m_loc__
#define m_loc__

#include "m_general.h"
#include "m_wii.h"

unsigned char getData(unsigned int* x, unsigned int* y);

unsigned char determine_blobs();

unsigned char local_init();

unsigned char localize (float* data);

unsigned int blobs[12];

float data[7];

#endif