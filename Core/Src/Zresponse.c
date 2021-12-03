#include "Zresponse.h"



void ads_get_all()
{
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	  HAL_SPI_TransmitReceive(&hspi3, txbuf, rxbuf, 2, 10);
	  //HAL_SPI_TransmitReceive_DMA(&hspi3, txbuf, rxbuf, 2);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	  ads_data[0] = *(uint16_t *)(&rxbuf[2]);
}


uint8_t triggerBuf=0;
uint8_t	triggerLevel=1;
void resTIM3()
{

	//软件判断触发
	if(triggerBuf == 0)
	{
		uint8_t triggerNow;
		triggerNow = HAL_GPIO_ReadPin(triggerInput_GPIO_Port, triggerInput_Pin);
		if(triggerNow == 1 && triggerLevel == 0)
		{
			//如果上一次检测电平为低电平，此次检测为高电平，触发成功
			triggerBuf = 1;
			triggerLevel = 1;
			TIM3->ARR = 13124;
			TIM3->CNT = 0;
		}
		else
		{
			//否则等待下次触发
			triggerLevel = triggerNow;
			return;
		}
	}

	//采样，下次进入定时器中断时才采样成功
	ads_get_all();

	//灯光闪烁
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

	if(catchCounts>0)
	{
		//从第一次开始采样
		if(SysMode == 0)
		{
			dataCatch[catchCounts-1] = dataCatch[catchCounts-1]  + ( ((float)(ads_data[0])) -32767.5 );
		}
		else
		{
			dataCatch[catchCounts-1] = dataCatch[catchCounts-1]  + ( ((float)(ads_data[0])) -32767.5 );
		}
	}



	if( ++catchCounts >128 )
	{
		//一个周期采样完成
		catchCounts = 0;
		HAL_TIM_Base_Stop_IT(&htim3);
		//进入做些什么函数
		triggerBuf = 0;
		dosomethingTIM3();
	}


}
