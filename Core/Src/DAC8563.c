#include "DAC8563.h"

void dac_delay_us(uint32_t length)
{
length = length*12;
while(length--);
}
void DAC8563_WRITE(uint8_t cmd,uint16_t data)
{
uint8_t s=0;
DAC8563_SYNC_H;
dac_delay_us(4);
DAC8563_SYNC_L;
DAC8563_SCLK_L;
for( s=0;s<8;s++)
{
	if((cmd&0x80)==0x80){DAC8563_DIN_H;}
	else{DAC8563_DIN_L;}
	dac_delay_us(4);
	DAC8563_SCLK_H;
	dac_delay_us(4);
	cmd<<=1;
	DAC8563_SCLK_L;
	dac_delay_us(4);
	}
	for( s=0;s<16;s++)
	{
	if((data&0x8000)==0x8000){DAC8563_DIN_H;}
	else{DAC8563_DIN_L;}
	dac_delay_us(4);
	DAC8563_SCLK_H;
	dac_delay_us(4);
	data<<=1;
	DAC8563_SCLK_L;
	dac_delay_us(4);
	}
	}
	void DAC8563_GPIO_Init(void)
	{
//	GPIO_InitTypeDef GPIO_Initure;
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOE_CLK_ENABLE();
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	__HAL_RCC_GPIOI_CLK_ENABLE();
//	GPIO_Initure.Pin=GPIO_PIN_0;
//	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
//	GPIO_Initure.Pull=GPIO_NOPULL;
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH; //⾼速
//	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//	GPIO_Initure.Pin=GPIO_PIN_11;
//	HAL_GPIO_Init(GPIOH,&GPIO_Initure);
//	GPIO_Initure.Pin=GPIO_PIN_1;
//	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
//	GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_8;
//	HAL_GPIO_Init(GPIOI,&GPIO_Initure);
	DAC8563_LDAC_H;
	}
	void DAC8563_Init(void)
	{
	DAC8563_CLR_L;
	DAC8563_LDAC_H;
	DAC8563_GPIO_Init();
	DAC8563_WRITE(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG); //复位
	HAL_Delay(10);
	DAC8563_WRITE(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B); //上电DAC-A和DAC-B
	DAC8563_WRITE(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);// 启⽤内部参考和复位DAC以获得增益= 2
	DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A2); // Set Gains
	HAL_Delay(200);
	DAC_OutAB(0,0);
	}
	//=============================================
	//
	// 设置并更新DAC输出电压（更改模块跳线帽及设置DAC8563内部增益 可设置不同输出范围）
	//
	// 参数：data_a为A路输出，data_b为B路输出
	//
	//=============================================
	#define mV_to_DA 3.2768f
	void DAC_OutA(float data_a) //单位 mV
	{
	DAC8563_WRITE(CMD_SETA_UPDATEA, data_a*mV_to_DA+32767);
	DAC8563_LDAC_L;
	dac_delay_us(4);
	DAC8563_LDAC_H;
	}
	void DAC_OutB(float data_b) //单位 mV
	{
	DAC8563_WRITE(CMD_SETB_UPDATEB, data_b*mV_to_DA+32767);
	DAC8563_LDAC_L;
	dac_delay_us(4);
	DAC8563_LDAC_H;
	}
	void DAC_OutAB(float data_a, float data_b) //单位 mV
	{
	DAC8563_WRITE(CMD_SETA_UPDATEA, data_a*mV_to_DA+32767);
	DAC8563_WRITE(CMD_SETB_UPDATEB, data_b*mV_to_DA+32767);
	DAC8563_LDAC_L;
	dac_delay_us(4);
	DAC8563_LDAC_H;
	}



