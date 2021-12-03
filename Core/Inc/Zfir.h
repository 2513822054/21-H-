#ifndef ZFIR_H
#define ZFIR_H
#include "Zworkman.h"

void Fir_Do(float *firInput,float* firOutput,uint32_t blockSize,uint32_t len);
void Fir_Realtime_Init();
void Fir_Realtime(float *firInput,float* firOutput);
#endif
