#include "AD9959.H"
//#include "Algorithm.h"
sweep_freq sweepfreq;
uint8_t CSR_DATA0[1] = {0x10}; // 开 CH0
uint8_t CSR_DATA1[1] = {0x20}; // 开 CH1
uint8_t CSR_DATA2[1] = {0x40}; // 开 CH2
uint8_t CSR_DATA3[1] = {0x80}; // 开 CH3
uint8_t FR2_DATA[2] = {0x00,0x00}; //default Value = 0x0000
uint8_t CFR_DATA[3] = {0x00,0x03,0x02}; //default Value = 0x000302
uint8_t CPOW0_DATA[2] = {0x00,0x00}; //default Value = 0x0000 @ =POW/2^14*360
uint8_t LSRR_DATA[2] = {0x00,0x00}; //default Value = 0x----
uint8_t RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
uint8_t FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------

//延时
void dds_delay(u32 length)
{
	length = length*12;
	while(length--);
}
//IO⼝初始化
void Intserve(void)
{
//GPIO_InitTypeDef GPIO_Initure;
//__HAL_RCC_GPIOA_CLK_ENABLE();
//__HAL_RCC_GPIOB_CLK_ENABLE();
//__HAL_RCC_GPIOE_CLK_ENABLE();
//__HAL_RCC_GPIOH_CLK_ENABLE();
//__HAL_RCC_GPIOG_CLK_ENABLE();
//GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
//GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
//GPIO_Initure.Pull=GPIO_NOPULL;
//GPIO_Initure.Speed=GPIO_SPEED_HIGH; //⾼速
//HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_12;
//HAL_GPIO_Init(GPIOB,&GPIO_Initure);
//GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
//HAL_GPIO_Init(GPIOE,&GPIO_Initure);
//GPIO_Initure.Pin=GPIO_PIN_3;
//HAL_GPIO_Init(GPIOG,&GPIO_Initure);
//GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_6|GPIO_PIN_7;
//HAL_GPIO_Init(GPIOH,&GPIO_Initure);

//AD9959_PWR_L;
	AD9959_CS_H;
	AD9959_SCLK_L;
	AD9959_UPDATE_L;
//AD9959_PS0_L;
//AD9959_PS1_L;
//AD9959_PS2_L;
//AD9959_PS3_L;
	AD9959_SDIO0_L;
//AD9959_SDIO1_L;
//AD9959_SDIO2_L;
//AD9959_SDIO3_L;
}
//AD9959复位
void IntReset(void)
{
	AD9959_Reset_L;
	dds_delay(1);
	AD9959_Reset_H;
	dds_delay(30);
	AD9959_Reset_L;
}
//AD9959更新数据
void IO_Update(void)
{
	AD9959_UPDATE_L;
	dds_delay(2);
	AD9959_UPDATE_H;
	dds_delay(4);
	AD9959_UPDATE_L;
}
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8*RegisterData,u8 temp)
{
	u8 ControlValue = 0;
	u8 ValueToWrite = 0;
	u8 RegisterIndex = 0;
	u8 i = 0;

	ControlValue = RegisterAddress;
	//写⼊地址
	AD9959_SCLK_L;
	AD9959_CS_L;
	for(i=0; i<8; i++)
	{
		AD9959_SCLK_L;
		if(0x80 == (ControlValue & 0x80))
			AD9959_SDIO0_H;
		else
			AD9959_SDIO0_L;
		AD9959_SCLK_H;
		ControlValue <<= 1;
	}
	AD9959_SCLK_L;
	//写⼊数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters;RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			AD9959_SCLK_L;
			if(0x80 == (ValueToWrite & 0x80))
				AD9959_SDIO0_H;
			else
				AD9959_SDIO0_L;
			AD9959_SCLK_H;
			ValueToWrite <<= 1;
		}

		AD9959_SCLK_L;
	}
	if(temp==1)
		IO_Update();
	AD9959_CS_H;
}
/*---------------------------------------
函数功能：设置通道输出频率
Channel: 输出通道
Freq: 输出频率
---------------------------------------*/
void Write_frequence(u8 Channel,u32 Freq)
{
	u8 CFTW0_DATA[4] ={0x00,0x00,0x00,0x00}; //中间变量
	u32 Temp;
	Temp=(u32)Freq*8.589934592; //将输⼊频率因⼦分为四个字节4.294967296=(2^32)/500000000
	CFTW0_DATA[3]=(u8)Temp;
	CFTW0_DATA[2]=(u8)(Temp>>8);
	CFTW0_DATA[1]=(u8)(Temp>>16);
	CFTW0_DATA[0]=(u8)(Temp>>24);
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//控制寄存器写⼊CH0通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address0x04.输出CH0设定频率
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//控制寄存器写⼊CH1通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address0x04.输出CH1设定频率
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//控制寄存器写⼊CH2通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address0x04.输出CH2设定频率
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//控制寄存器写⼊CH3通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);//CTW0 address0x04.输出CH3设定频率
	}
}
/*---------------------------------------
函数功能：设置通道输出幅度
Channel: 输出通道
Ampli: 输出幅度
---------------------------------------*/
void Write_Amplitude(u8 Channel, u16 Ampli)
{
	u16 A_temp;//=0x23ff;
	u8 ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest= 18.91/Iout
	A_temp=Ampli|0x1000;
	ACR_DATA[2] = (u8)A_temp; //低位数据
	ACR_DATA[1] = (u8)(A_temp>>8); //⾼位数据
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
}
/*---------------------------------------
函数功能：设置通道输出相位
Channel: 输出通道
Phase: 输出相位,范围：0~16383(对应⾓度：0°~360°)
---------------------------------------*/
void Write_Phase(u8 Channel,u16 Phase)
{
	u16 P_temp=0;
	P_temp=(u16)Phase;
	CPOW0_DATA[1]=(u8)P_temp;
	CPOW0_DATA[0]=(u8)(P_temp>>8);
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
}
//AD9959初始化
void Init_AD9959(void)
{
	uint16_t phase;
	uint32_t freq;
	float amp;
	u8 FR1_DATA[3] = {0xD0,0x00,0x00};//20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;
	Intserve(); //IO⼝初始化
	IntReset(); //AD9959复位
	WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//写功能寄存器1
// WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);
// WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
// WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
// WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
// WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
// WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
// WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
//写⼊初始频率
	sweepfreq.flag=0;
	sweepfreq.start=1000;
	sweepfreq.step=1000;
	sweepfreq.time=100;
	sweepfreq.end=1000000;
	freq = 1000;
	phase = 0;
	amp = 500;
	Out_freq(0,freq);
	Write_Phase(0, phase);
	Out_mV(0, amp);
	HAL_Delay(200);
}
void Out_mV(u8 ch, float nmV)
{
	nmV=nmV/500*1024;
	if(nmV>1023)nmV=1023;//范围保护
	Write_Amplitude(ch,nmV);
}
void Out_freq(u8 ch, u32 freq)
{
	if(freq>FreqMax)freq=FreqMax;
	if(freq<FreqMin)freq=FreqMin;//范围保护
	Write_frequence(ch,freq);
}
//扫频函数
/*
更改好sweepfreq结构体的参数后，直接调⽤本函数将扫频⽣效
*/
void Sweep_Freq(void)
{
	u16 i,times;
	times=(sweepfreq.end-sweepfreq.start)/sweepfreq.step+1;
	for(i=0;i<times;i++)
	{
		Out_freq(0,sweepfreq.start+sweepfreq.step*i);
		delay_ms(sweepfreq.time);
	}
}
