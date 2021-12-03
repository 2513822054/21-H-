#ifndef __WORKMAN_H_
#define __WORKMAN_H_
#include "ad9959.h"
#include "ADS8688.h"
#include "DAC8563.h"
#include "outputdata.h"
#include "tft.h"
#include "Zdosomething.h"
#include "Zresponse.h"
#include "W25Q128.h"
#include "WIFI.h"
#include "main.h"
#include "arm_math.h"
#include "Ztest.h"
#include "Zfir.h"
#define CATCH_NUM 128            //采样点数

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac1;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim14;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;



//已学习数据
extern float dataTimSample[7][CATCH_NUM];           //时域信息
extern float dataFreqSample[7][10];		   //频域信息（向量）
extern float dataParaSample[7][20];           //参数信息，依次为：0电压峰峰值，1能量，2有功功率，3电流有效值，4最大值，5最小值，后面待定

//现有数据
extern float dataTimNow[CATCH_NUM];
extern float dataFreqNow[128];
extern float dataParaNow[20];

//当前采样数据
extern float dataCatch[CATCH_NUM];
extern uint32_t catchTimes,catchCounts;       //采样次数，采样点数

//系统模式   0:分析模式   1:学习模式
extern uint8_t SysMode;
//6电器是否是小电器    0:不是    1:是
extern uint8_t num6Small;

//分析模式参数    七种电器共有128种组合方式     一个八位变量，后七位上1表示该电器在电路中，0表示不在电路中
extern float dataVec[5][128][2];           //频域五次谐波向量
extern float selfDistance[5][2];				 //当前波五次谐波的模长
extern float dataVecDiffer[5][128][2];		//求差向量
extern float dataDistance[5][128*2];		 //频域基波距离（后一半无用）
extern float dataDisLoss[6][128];          //频域基波损失
extern float dataTimLoss[128];			 //时域损失
extern float dataTimDisLoss[128];	     //频域时域损失加权和
extern float weight[2];				 //时域和频域的权重(通过学习求得)
extern uint32_t fittest;				 //最适组合（频域时域损失加权和最小）

extern arm_rfft_fast_instance_f32 fft_S;
extern uint32_t sectorBuf;

extern uint8_t minDisPlace[6],minTimPlace,minTimDisPlace;
extern float minDis[6],minTim,minTimDis;

void get_ads_alldata();

//初始化自己外设及while循环前的函数
void init_start();
//while循环里做的函数
void while_do();

//毫秒级延迟函数      需要打开14定时器
void delay_us(uint16_t us);



#endif
