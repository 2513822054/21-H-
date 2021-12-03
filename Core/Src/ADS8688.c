#include "ADS8688.h"

uint16_t ads_data[8];
uint8_t txbuf[4];
uint8_t rxbuf[4];

ADS8688 ads;

ADS8688 ads8688;
/*
 * INITIALISATION
 */
uint8_t ADS8688_Init(ADS8688 *ads, SPI_HandleTypeDef *spiHandle, GPIO_TypeDef *csPinBank, uint16_t csPin) {
/* Store interface parameters in struct */
	ads->spiHandle 		= spiHandle;
	ads->csPinBank   	= csPinBank;
	ads->csPin 		   = csPin;

	uint8_t ads_data[2] = {0};
	uint8_t state = 0;
	// reset all registers to default
	state += ADS_Cmd_Write(ads, RST, ads_data);
	HAL_Delay(100);
	// send a no_op message to the ADS to enter IDLE mode
	state += ADS_Cmd_Write(ads, CONT, ads_data);
	HAL_Delay(10);
	// enable auto transmit for all inputs(datasheet page 54) or as many as you want
	// if you want only some of the inputs enabled, make sure to power down the unused ones
	ads_data[0] = 0x01;
	state += ADS_Prog_Write(ads, AUTO_SEQ_EN, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0xFe;
	state += ADS_Prog_Write(ads, CHN_PWRDN, ads_data);
	HAL_Delay(10);
	// set the desired features such as device id (if multiple devices are used), alarm enable/disable and output format
	ads_data[0] = 0x03; // here i chose id = 0, alarm = disabled and SDO_format = 3 (datasheet page 56)
	state += ADS_Prog_Write(ads, FEATURE_SELECT, ads_data);
	HAL_Delay(10);
	// set all channels ranges(page 57)
	// 0x05 -> Input range is set to 0 to 2.5 x VREF (for VREF=5 volts, this means 0-10 volts range)
	// 0x06 -> Input range is set to 0 to 1.25 x VREF (for VREF=5 volts, this means 0-5 volts range)
	ads_data[0] = 0x01;
	state += ADS_Prog_Write(ads, CHN_0_RANGE, ads_data);
	HAL_Delay(10);
//	ads_data[0] = 0x02;
//	state += ADS_Prog_Write(ads, CHN_1_RANGE, ads_data);
//	HAL_Delay(10);
//	ads_data[0] = 0x05;
//	state += ADS_Prog_Write(ads, CHN_2_RANGE, ads_data);
//	HAL_Delay(10);
//	ads_data[0] = 0x06;
//	state += ADS_Prog_Write(ads, CHN_3_RANGE, ads_data);
//	HAL_Delay(10);



	// start the auto transmission by entering the appropriate state
	state += ADS_Cmd_Write(ads, AUTO_RST, ads_data);

	return state;
}



// after the read, data contains the data from the addressed register
HAL_StatusTypeDef ADS_Prog_Read(ADS8688 *ads, uint8_t addr, uint8_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t txbuf[2] = {0x00, (addr<<1 & 0xfe)}; // [15:9]->address, [8]->0, [7:0]->don't care (0x00) (stm32 uses little endian so reverse it)
	uint8_t rxbuf[4];

	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_RESET);
	ret = HAL_SPI_TransmitReceive(ads->spiHandle, txbuf, rxbuf, 2, 10);
	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_SET);

	data[0] = rxbuf[2];
	data[1] = rxbuf[3];
	return ret;
}

// after the write, data should contain the data (byte) written to the addressed register (check equality for evaluation)
HAL_StatusTypeDef ADS_Prog_Write(ADS8688 *ads, uint8_t addr, uint8_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t txbuf[2] = {data[0], (addr << 1 | 0x01)}; // [15:9]->address[6:0], [8]->1, [7:0]->data[7:0] (stm32 uses little endian so reverse it)
	uint8_t rxbuf[4];

	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_RESET);
	ret = HAL_SPI_TransmitReceive(ads->spiHandle, txbuf, rxbuf, 2, 10);
	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_SET);

	data[0] = rxbuf[3];
	data[1] = 0x00;
	return ret;
}

HAL_StatusTypeDef ADS_Cmd_Write(ADS8688 *ads, uint8_t cmd, uint8_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t txbuf[2] = {0x00,cmd}; // [15:9]->address[6:0], [8]->1, [7:0]->data[7:0] (stm32 uses little endian so reverse it)
	uint8_t rxbuf[4];

	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_RESET);
	ret = HAL_SPI_TransmitReceive(ads->spiHandle, txbuf, rxbuf, 2, 10);
	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_SET);

	data[0] = rxbuf[2];
	data[1] = rxbuf[3];
	return ret;
}

HAL_StatusTypeDef ADS_Read_All_Raw(ADS8688 *ads, uint16_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t ads_raw[2];
	for(int i=0; i<CHNS_NUM_READ; i++) {
	  ret = ADS_Cmd_Write(ads, CONT, ads_raw);
	  data[i] = (int)((uint16_t)(ads_raw[1]<<8 | ads_raw[0]) >> 4);
	}
	return ret;
}

//
//
//Niko 2021/7/14 17:43:23
//#define NO_OP			0x0000 /*在选定模式下继续操作
//													在设备操作期间将SDI线连续保持为低电平（相当于向所有16位写入0）将继续在最后选择的模式（STDBY，PWR_DN或AUTO_RST，MAN_Ch_n）下进行设备操作。
//													在这种模式下，设备将遵循程序寄存器（地址01h至3Ch）中已配置的相同设置。*/
//#define STDBY			0x8200 /*待机模式
//													器件支持低功耗待机模式（STDBY），在该模式下，仅部分电路断电。
//													内部基准电压源和缓冲器未断电，在退出STDBY模式时，可以在20 μs内为器件加电。退出STDBY模式时，程序寄存器不会复位为默认值。*/
//#define PWR_DN  	0x8300/*掉电模式（PWR_DN）
//													器件支持硬件和软件掉电模式（PWR_DN），在该模式下，所有内部电路均被掉电，包括内部基准电压源和缓冲器。
//													如果设备在内部参考模式下运行（REFSEL = 0），则退出PWR_DN模式后，设备加电并转换所选模拟输入通道至少需要15 ms的时间。
//													设备的硬件电源模式在RST / PD（输入）部分中说明。
//													硬件和软件掉电模式之间的主要区别在于，当设备从硬件掉电唤醒时，程序寄存器会重置为默认值，但是当设备从软件唤醒时，程序寄存器的先前设置会保留掉电。*/
//#define RST				0x8500/*重置程序寄存器（RST）
//													器件支持硬件和软件复位（RST）模式，在该模式下，所有程序寄存器均复位为其默认值。也可以使用硬件引脚将设备置于RST模式*/
//#define AUTO_RST	0xA000/*自动通道启用
//													自动扫描所有模拟信道上的输入信号。自动扫描的通道序列可由程序寄存器中的自动扫描顺序控制寄存器（01h至02h）配置；请参阅程序寄存器映射部分。
//													在此模式下，设备以升序连续循环通过所选通道，从最低通道开始，转换程序寄存器中选择的所有通道。
//													序列完成后，设备返回到程序寄存器中的最低计数通道，并重复该序列。通过设置程序寄存器的范围选择寄存器，可以配置自动扫描序列中每个通道的输入电压范围。*/
//
//Niko 2021/7/14 17:43:29
//#define MAN_CH_0	0xC000/**/
//#define MAN_CH_1	0xC400/**/
//#define MAN_CH_2	0xC800/**/
//#define MAN_CH_3	0xCC00/**/
//#define MAN_CH_4	0xD000/**/
//#define MAN_CH_5	0xD400/**/
//#define MAN_CH_6	0xD800/**/
//#define MAN_CH_7	0xDC00/*手动通道n选择（MAN_Ch_n）
//												通过在手动通道n扫描模式（MAN_Ch_n）中进行操作，可以对设备进行编程以转换特定的模拟输入通道。通过在命令寄存器中写入有效的手动通道n选择命令（MAN_Ch_n）来完成该编程*/
//#define MAN_AUX		0XE000/*AUX通道*/
//
////程序寄存器
//#define AUTO_SEQ_EN	0x01	/*自动扫描序列使能寄存器
//													该寄存器选择单个通道以在AUTO_RST模式下进行排序。
//													该寄存器的默认值为FFh，这意味着在默认条件下所有通道都包含在自动扫描序列中。*/
//#define CH_PD				0x02	/*通道掉电寄存器
//													该寄存器关闭掉AUTO_RST模式下不包含的单个通道的电源。 该寄存器的默认值为00h，这意味着在默认条件下所有通道均已上电。*/
//#define F_S					0X03	/*器件功能选择控制寄存器
//													该寄存器中的位可用于配置菊花链操作的设备ID，并配置SDO上的输出位格式。*/
//
////各通道范围选择寄存器	默认值为00h
//#define CHIR_0		0x05/**/
//#define CHIR_1		0x06/**/
//#define CHIR_2		0x07/**/
//#define CHIR_3		0x08/**/
//#define CHIR_4		0x09/**/
//#define CHIR_5		0x0A/**/
//#define CHIR_6		0x0B/**/
//#define CHIR_7		0x0C/**/
//
//Niko 2021/7/14 17:43:36
//#define CMD_READ	0x3F	/*命令回读寄存器
//												该寄存器允许用户读取设备的操作模式。 执行此命令后，设备将输出在前一个数据帧中执行的命令字。
//												有关命令寄存器的所有信息在前8位中包含，并且后8位为0。*/
//
///*************************************************************************************************************/
//#define CH7_EN  0X80
//#define CH6_EN  0X40
//#define CH5_EN  0X20
//#define CH4_EN  0X10
//#define CH3_EN  0X08
//#define CH2_EN  0X04
//#define CH1_EN  0X02
//#define CH0_EN  0X01
//
//#define REF 4096
////输入范围
//#define ADS8688_IR_N2_5V    0x00  //±2.5*ref  ref=4.096V  ±10.24V
//#define ADS8688_IR_N1_25V   0x01  //±1.25*ref    ±5.12V
//#define ADS8688_IR_N0_625V  0x02  //±0.625*ref   ±2.56V
//#define ADS8688_IR_2_5V   	0x05  //0-2.5*ref      0-10.24V
//#define ADS8688_IR_1_25V    0x06  //0-1.25*ref     0-5.12V
//
//#define CONST_N2_5V_LSB_mV  	2.5*2*REF/65535
//#define CONST_N1_25V_LSB_mV  	1.25*2*REF/65535
//#define CONST_N0_625V_LSB_mV  0.625*2*REF/65535
//#define CONST_2_5V_LSB_mV  		2.5*REF/65535
//#define CONST_1_25V_LSB_mV 	 	1.25*REF/65535
//
//


