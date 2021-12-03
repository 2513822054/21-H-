#ifndef __IIR_H_
#define __IIR_H_
#include "main.h"
void iir_init();
void iir_Do(float *Input,float *Output,uint32_t len);
void iir_Do_Realtime(float *Input,float *Output);



#endif
