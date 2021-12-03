#ifndef _outputdata_H
#define _outputdata_H
#include "main.h"

extern float OutData[4];
void OutPut_Data(void);

/*
 * channel  通道号   0-3
 * *Pdata   	数据地址
 * *len	        数据长度
 */
void OutputOne(uint8_t channel,float *Pdata,uint32_t len);

/*
 * channel  通道号   0-3
 * *Pdata   	数据地址
 * *len	        数据长度
 */
void OutputAll(float *Pdata0,float *Pdata1,float *Pdata2,float *Pdata3,uint32_t len);

#endif 
