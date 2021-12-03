#include "outputdata.h"
//#include "uart.h"

float OutData[4] = { 0 };

unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
	HAL_UART_Transmit(TFT_SEND,&databuf[i], 1, 2);
    //uart_putchar(databuf[i]);
}




void OutputOne(uint8_t channel,float *Pdata,uint32_t len)
{
	uint32_t i;
	for(i=0;i<len;i++)
	{
		OutData[channel] = Pdata[i];
		OutPut_Data();
	}
	OutData[channel]=0;
}

void OutputAll(float *Pdata0,float *Pdata1,float *Pdata2,float *Pdata3,uint32_t len)
{
	uint32_t i;
	for(i=0;i<len;i++)
	{
		OutData[0] = Pdata0[i];
		OutData[1] = Pdata1[i];
		OutData[2] = Pdata2[i];
		OutData[3] = Pdata3[i];
		OutPut_Data();
	}
	OutData[0]=0;
	OutData[1]=0;
	OutData[2]=0;
	OutData[3]=0;

}







