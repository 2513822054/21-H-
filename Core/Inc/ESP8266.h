#ifndef __ESP8266_H
#define __ESP8266_H
#include "main.h"
#define WIFI_UART &huart6

//串口接收存储一个字节函数
void WIFI_Receive_One();
//串口接收处理函数
void WIFI_Receive_Handle(uint8_t status);
void WIFI_Receive_Data(uint8_t *pdata,uint8_t chl,uint16_t len);
uint8_t WIFI_Transmit_Data(uint8_t * pdata,uint8_t len);

void WIFI_TCP_Server_Init(uint8_t isRST);
void WIFI_TCP_Client_Init(uint8_t isRST);

#endif
