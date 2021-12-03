#ifndef _AD9959_H_
#define _AD9959_H_
#include "main.h"
#include "stdint.h"
//AD9959寄存器地址定义
#define CSR_ADD 0x00 //CSR 通道选择寄存器
#define FR1_ADD 0x01 //FR1 功能寄存器1
#define FR2_ADD 0x02 //FR2 功能寄存器2
#define CFR_ADD 0x03 //CFR 通道功能寄存器
#define CFTW0_ADD 0x04 //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05 //CPW0 通道相位转换字寄存器
#define ACR_ADD 0x06 //ACR 幅度控制寄存器
#define LSRR_ADD 0x07 //LSR 通道线性扫描寄存器
#define RDW_ADD 0x08 //RDW 通道线性向上扫描寄存器
#define FDW_ADD 0x09 //FDW 通道线性向下扫描寄存器
//AD9959管脚宏定义
//#define AD9959_PS0_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
//#define AD9959_PS1_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)
//#define AD9959_PS2_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET)
//#define AD9959_PS3_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET)
//#define AD9959_SDIO1_L HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET)
//#define AD9959_SDIO2_L HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_RESET)
//#define AD9959_SDIO3_L HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_RESET)
//#define AD9959_PWR_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET)

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define AD9959_SDIO0_L HAL_GPIO_WritePin(AD9959_SD0_GPIO_Port,AD9959_SD0_Pin,RESET)
#define AD9959_SDIO0_H HAL_GPIO_WritePin(AD9959_SD0_GPIO_Port,AD9959_SD0_Pin,SET)

#define AD9959_Reset_H HAL_GPIO_WritePin(AD9959_RST_GPIO_Port,AD9959_RST_Pin,SET)
#define AD9959_Reset_L HAL_GPIO_WritePin(AD9959_RST_GPIO_Port,AD9959_RST_Pin,RESET)

#define AD9959_UPDATE_H HAL_GPIO_WritePin(AD9959_UP_GPIO_Port,AD9959_UP_Pin,SET)
#define AD9959_UPDATE_L HAL_GPIO_WritePin(AD9959_UP_GPIO_Port,AD9959_UP_Pin,RESET)

#define AD9959_CS_H HAL_GPIO_WritePin(AD9959_CS_GPIO_Port,AD9959_CS_Pin,SET)
#define AD9959_CS_L HAL_GPIO_WritePin(AD9959_CS_GPIO_Port,AD9959_CS_Pin,RESET)

#define AD9959_SCLK_H HAL_GPIO_WritePin(AD9959_SCK_GPIO_Port,AD9959_SCK_Pin,SET)
#define AD9959_SCLK_L HAL_GPIO_WritePin(AD9959_SCK_GPIO_Port,AD9959_SCK_Pin,RESET)

#define FreqMax 40000000.0f
#define FreqMin 0.0f

typedef struct _sweep_freq //正弦波形结构体
{
	uint32_t flag; //是否使能
	uint32_t start; //起始Hz
	uint32_t step; //步进Hz
	uint32_t time; //单次停留时间
	uint32_t end; //终点Hz
}sweep_freq;

extern sweep_freq sweepfreq;
void Out_freq(uint8_t ch, uint32_t freq);
void Out_mV(uint8_t ch, float nmV);
void Init_AD9959(void);
void Sweep_Freq(void);
#endif

