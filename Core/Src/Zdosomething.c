#include "Zdosomething.h"
#include <math.h>
const float Vcity[128] = {0.000000,15.266278,30.495777,45.651810,60.697863,75.597690,90.315396,104.815524,119.063142,133.023927,146.664245,159.951236,172.852891,185.338128,197.376869,208.940112,220.000000,230.529889,240.504411,249.899537,258.692633,266.862515,274.389503,281.255462,287.443852,292.939765,297.729960,301.802898,305.148766,307.759504,309.628822,310.752218,311.126984,310.752218,309.628822,307.759504,305.148766,301.802898,297.729960,292.939765,287.443852,281.255462,274.389503,266.862515,258.692633,249.899537,240.504411,230.529889,220.000000,208.940112,197.376869,185.338128,172.852891,159.951236,146.664245,133.023927,119.063142,104.815524,90.315396,75.597690,60.697863,45.651810,30.495777,15.266278,0.000000,-15.266278,-30.495777,-45.651810,-60.697863,-75.597690,-90.315396,-104.815524,-119.063142,-133.023927,-146.664245,-159.951236,-172.852891,-185.338128,-197.376869,-208.940112,-220.000000,-230.529889,-240.504411,-249.899537,-258.692633,-266.862515,-274.389503,-281.255462,-287.443852,-292.939765,-297.729960,-301.802898,-305.148766,-307.759504,-309.628822,-310.752218,-311.126984,-310.752218,-309.628822,-307.759504,-305.148766,-301.802898,-297.729960,-292.939765,-287.443852,-281.255462,-274.389503,-266.862515,-258.692633,-249.899537,-240.504411,-230.529889,-220.000000,-208.940112,-197.376869,-185.338128,-172.852891,-159.951236,-146.664245,-133.023927,-119.063142,-104.815524,-90.315396,-75.597690,-60.697863,-45.651810,-30.495777,-15.266278};
//求参数
float fftBuf[128],fftVec[128];
float dataAC[128],dataReal[128],dataUI[128];
float dataAvg,dataMax,dataMin,dataRms;
float fftScalar[128];
uint32_t dataBuf;

void do_get_para()
{

	uint32_t i;

	arm_copy_f32(dataCatch, dataTimNow,128);
	//arm_sub_f32(dataTimNow, &dataTimSample[6], dataTimNow, 128);


	arm_scale_f32(dataCatch, 5.12/32767.5 * 2 *2  , dataReal,128);
	//arm_scale_f32(dataTimNow, 5.12/32768 * 2 *2  , dataReal,128);

	//arm_sub_f32(dataTimNow, &dataTimSample[6], dataTimNow, 128);
	//arm_scale_f32(dataCatch, 5.12/32768 * 2 *2  , dataReal,128);

	//arm_scale_f32(dataReal, 1000, dataReal,128);
	//arm_scale_f32(dataCatch, 10.24/3276.8, dataTimNow,128);


	//fft求出时域
	arm_copy_f32(dataReal, fftBuf,128);			//拷贝数组
	//arm_scale_f32(dataCatch, 10000, dataReal,128);
	arm_scale_f32(fftBuf, 100000, fftBuf,128);
	arm_rfft_fast_f32(&fft_S, fftBuf, fftVec,0);		//fft求出向量
	arm_copy_f32(&fftVec[2], dataFreqNow,128);   //暂存前五个向量
	//基频相位
	dataParaNow[9] = atan2f(fftVec[2],fftVec[3]) * 180 / 3.1415926;

	//求出模长
	arm_cmplx_mag_f32(fftVec, fftScalar, 128);

	//求其他参数
	//平均值
	arm_mean_f32(dataReal,CATCH_NUM,&dataAvg);
	dataParaNow[6] = dataAvg;
	//最大值最小值峰峰值
	arm_max_f32(dataReal, CATCH_NUM, &dataMax, &dataBuf);
	arm_min_f32(dataReal, CATCH_NUM, &dataMin, &dataBuf);
	dataParaNow[0] = dataMax-dataMin;
	dataParaNow[4] = dataMax;
	dataParaNow[5] = dataMin;
	//有效值
	arm_rms_f32(dataReal, CATCH_NUM, &dataRms);
	//arm_sqrt_f32(dataRms,&dataRms);
	if(dataRms < 0.039  && dataRms > 0.0010)
	{
		dataRms = dataRms*(70000*dataRms*dataRms*dataRms - 6700*dataRms*dataRms + 205.7*dataRms - 1.1737);
	}
	dataParaNow[3] = dataRms;
	//市载功率
	dataParaNow[7] = dataParaNow[3] *220;
	//失真度
	float fenzi;
	arm_power_f32(&(fftScalar[2]),62,&fenzi);
	arm_sqrt_f32(fenzi, &fenzi);
	dataParaNow[8] = fenzi/fftScalar[1]*100;



}


//最小二乘法函数
float min_two(float *Psample , float *Pnow)
{
	float weight[128],abs[128],max,sub[128],subweight[128];
	uint32_t maxplace,i;
	float result,rms;
//	//求出最小二乘法的权重矩阵
//	arm_abs_f32(Psample, abs, 128);            //求绝对值
//	arm_max_f32(abs, 128, &max, &maxplace);    //求绝对值后求最大值
//	for(i=0;i<128;i++)
//	{
//		if(abs[i] < (max/10))weight[i] = 10;
//		else
//		{
//			weight[i] = max/abs[i];
//		}
//
//		//weight[i] = 1;
//	}

	arm_sub_f32(Pnow, Psample, sub, 128);
	//arm_mult_f32(sub, weight, subweight, 128);
	arm_rms_f32(sub, 128, &result);
	return result;

}

uint32_t timcount;
//器件分析
void do_analysis()
{
	uint32_t i=0;
	uint8_t k=0;
	uint32_t placeBuf;
	float ratio;
	float sample[128]={0};

	//当前向量和样本向量求差
	for(i=0;i<128;i++)
	{
		dataVecDiffer[0][i][0] = dataVec[0][i][0] - dataFreqNow[0];
		dataVecDiffer[0][i][1] = dataVec[0][i][1] - dataFreqNow[1];

		dataVecDiffer[1][i][0] = dataVec[1][i][0] - dataFreqNow[2];
		dataVecDiffer[1][i][1] = dataVec[1][i][1] - dataFreqNow[3];

		dataVecDiffer[2][i][0] = dataVec[2][i][0] - dataFreqNow[4];
		dataVecDiffer[2][i][1] = dataVec[2][i][1] - dataFreqNow[5];

		dataVecDiffer[3][i][0] = dataVec[3][i][0] - dataFreqNow[6];
		dataVecDiffer[3][i][1] = dataVec[3][i][1] - dataFreqNow[7];

		dataVecDiffer[4][i][0] = dataVec[4][i][0] - dataFreqNow[8];
		dataVecDiffer[4][i][1] = dataVec[4][i][1] - dataFreqNow[9];
	}

	for(i=0;i<5;i++)
	{
		//求出所有向量差的模长
		arm_cmplx_mag_f32(&dataVecDiffer[i], &dataDistance[i], 128);
		//求出模长与本征向量相除求出损失值
		arm_cmplx_mag_f32(&dataFreqNow[i*2], &selfDistance[i], 1);
		ratio = 1/selfDistance[i][0];
		//arm_scale_f32(&dataDistance[i], ratio, dataDisLoss[i], 128);
		arm_scale_f32(&dataDistance[i], ratio, &dataDisLoss[i], 128);
		arm_min_f32(dataDisLoss[i], 128, &minDis[i], &placeBuf);
		minDisPlace[i] = (uint8_t)placeBuf;
	}
	//测试！！！
	//arm_mult_f32(dataDisLoss, dataDisLoss, dataDisLoss, 128);






	arm_min_f32(minDis, 5, &minDis[5], &placeBuf);
	minDisPlace[6] = placeBuf;


	TIM14->CNT=0;

	for(i=0;i<128;i++)
	{
		arm_fill_f32(0, sample, 128);
		for(k=0;k<7;k++)
		{
			if(  ((i>>k)&0x01)   )
			{
				arm_add_f32(sample, &dataTimSample[k] , sample, 128);
			}
		}
		dataTimLoss[i] = min_two(sample,dataTimNow);
	}
	arm_min_f32(dataTimLoss, 127, &minTim, &minTimPlace);
	arm_mult_f32(dataTimLoss, &dataDisLoss[0], dataTimDisLoss, 128);
	arm_min_f32(dataTimDisLoss, 127, &minTimDis, &minTimDisPlace);
	timcount = TIM14->CNT;

}
void show_As_One_1(uint8_t screenid,uint8_t controlid,float* pdata,uint16_t len)
{
	float ratio,max,min,Vpp,Vdc,databuf[len];
	uint32_t buf,i;
	uint8_t text[len];
	arm_copy_f32(pdata, databuf, 128);
	arm_max_f32(databuf,len,&max,&buf);
	arm_min_f32(databuf,len,&min,&buf);

	arm_offset_f32(databuf, -min, databuf, 128);
	max = max - min;
	ratio = 255/max;
	arm_scale_f32(databuf,ratio,databuf,len);



	for(i=0;i<len;i++)
	{
		text[i] = (uint8_t)(databuf[i]);
	}
	tft_form(screenid, controlid, text, len, 1);
}


void show_As_One(uint8_t screenid,uint8_t controlid,float* pdata,uint16_t len)
{
	float ratio,max,min,Vpp,Vdc,databuf[len];
	uint32_t buf,i;
	uint8_t text[len];
	arm_copy_f32(pdata, databuf, 128);
	arm_max_f32(databuf,len,&max,&buf);
	arm_min_f32(databuf,len,&min,&buf);

	arm_offset_f32(databuf, -min, databuf, 128);
	max = max - min;
	ratio = 255/max;
	arm_scale_f32(databuf,ratio,databuf,len);



	for(i=0;i<len;i++)
	{
		text[i] = (uint8_t)(databuf[i]);
	}
	tft_form(screenid, controlid, text, len, 0);

}

uint32_t countShow=0;
uint8_t lastEight[5],showPlace;

uint8_t get_most()
{

	uint8_t mostnum=1;
	uint8_t mostplace=0;
	uint8_t buf;
	uint8_t i,k;
	for(i=0;i<5;i++)
	{
		for(k=0;k<5;k++)
		{
			buf = 0;
			if(lastEight[i] == lastEight[k])
			{
				buf += 1;
			}
		}
		if(buf>mostnum)
		{
			mostnum = buf;
			mostplace = i;
		}
	}
	return lastEight[mostplace];
}

//显示
void do_show()
{
	//学习模式显示
	if(SysMode)
	{
		//将曲线归一化显示
		show_As_One(1, 2, dataTimNow, CATCH_NUM);
		//显示参数

	}
	//分析模式显示
	else
	{
		uint32_t i;
		//将曲线归一化显示
		show_As_One(0, 2, dataTimNow, CATCH_NUM);
		//显示参数
		tft_text_float_3(0, 3, dataParaNow[0]*0.66);
		tft_text_float_3(0, 4, dataParaNow[3]*0.66);
		tft_text_float_6(0, 5, dataParaNow[7]*0.66);
		tft_text_float_3(0, 6, dataParaNow[8]);
		tft_text_float_3(0, 7, dataParaNow[9]);
		tft_text_float_3(0, 8, dataParaNow[6]*0.66);

//		for(i=0;i<1;i++)
//		{
//			tft_text_u8_bit(0,16+i*2,minDisPlace[i]);
//			tft_text_float_6(0, 17+i*2, minDis[i]);
// 		}
//		tft_text_u8_bit(0, 24, minTimDisPlace);
//		tft_text_float_6(0, 25, minTimDis);
//
//		tft_text_u8_bit(0, 26, minTimPlace);
//		tft_text_float_6(0, 27, minTim);

		lastEight[countShow] = (uint8_t)minTimDisPlace;
		if(++countShow>=5)countShow = 0;
		showPlace = get_most();

		for(i=0;i<7;i++)
		{
			if(  ((uint8_t)showPlace >> i)   &   0x01   )
			{
				tft_button_press(0,10+i);
			}
			else
			{
				tft_button_lose(0,10+i);
			}
		}


		for(i=0;i<128;i++)
		{
			//printf("%d:\r\n%.6f\r\n%.6f\r\n",i,dataDisLoss[i],dataTimLoss[i]);
//			printf("%d:\r\n",i);
//			printf("%.6f\r\n",dataDisLoss[0][i]);
//			printf("%.6f\r\n",dataTimLoss[i]);
//			printf("%.6f\r\n",dataTimDisLoss[i]);
		}


	}

}

void dosomethingTIM3()
{
//	//如果数据全为0xFFFF或0x0000    重启AD8688
//	if( dataCatch[13]  >32765 && dataCatch[20]  >32765 && dataCatch[30]  >32765 && dataCatch[100]  >32765)
//	{
//		ADS8688_Init(&ads, &hspi3, SPI3_CS_GPIO_Port, SPI3_CS_Pin);  //初始化ADS8688
//		//如果采样次数不够，重新采样，够了就平均
//		catchTimes = 0;
//		TIM3->CNT = 0;
//		TIM3->ARR = 80;    //增加定时器频率触发
//		HAL_TIM_Base_Start_IT(&htim3);
//
//		return;
//	}


	if(++catchTimes<=5 && SysMode == 0)
	{
		//如果采样次数不够，重新采样，够了就平均
		TIM3->CNT = 0;
		TIM3->ARR = 80;    //增加定时器频率触发
		HAL_TIM_Base_Start_IT(&htim3);

		return;
	}
	else if(catchTimes<=5 && SysMode == 1)
	{
		//如果采样次数不够，重新采样，够了就平均
		TIM3->CNT = 0;
		TIM3->ARR = 80;    //增加定时器频率触发
		HAL_TIM_Base_Start_IT(&htim3);

		return;
	}
	if(SysMode == 0)
	{
		arm_scale_f32(dataCatch, 0.2, dataCatch, 128);
		//arm_offset_f32(dataCatch, -32767.5, dataCatch, 128);

	}
	else if(SysMode == 1)
	{
		arm_scale_f32(dataCatch, 0.2, dataCatch, 128);
		//arm_offset_f32(dataCatch, -32767.5, dataCatch, 128);

	}




	catchTimes = 0;

	do_get_para();

	if(SysMode == 0)
	{
		do_analysis();
	}

	do_show();




	//ADS8688故障检测并重启
	if(dataParaNow[4] > 8.195 && dataParaNow[5] > 8.195)ADS8688_Init(&ads, &hspi3, SPI3_CS_GPIO_Port, SPI3_CS_Pin);


	//ADS8688_Init(&ads, &hspi3, SPI3_CS_GPIO_Port, SPI3_CS_Pin);
	TIM3->CNT = 0;
	TIM3->ARR = 80;    //增加定时器频率触发
	HAL_TIM_Base_Start_IT(&htim3);

}




