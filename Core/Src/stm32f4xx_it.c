/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi3_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  /* USER CODE BEGIN TIM3_IRQn 1 */
  __HAL_TIM_CLEAR_FLAG(&htim3, TIM_SR_UIF);
  resTIM3();
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */
  if(tft_receive() == 1)
  {
	  //暗口重启
	  if(controlbuf.controlid == 99)
	  {

			W25QXX_Write(dataTimSample, sectorBuf*4096   ,  128*7*4);
			W25QXX_Write(dataFreqSample, (sectorBuf+1)*4096 , 10*7*4);
			W25QXX_Write(dataParaSample, (sectorBuf+2)*4096 , 20*7*4);
			W25QXX_Write(dataVec, (sectorBuf+3)*4096 , 5*128*2*4);
			tft_text(1, 20, "Finish!\0", 8);
			SysMode = 0;


		    __set_FAULTMASK(1);//关闭总中�??
		    NVIC_SystemReset();//请求单片机重�??
	  }
	  //切换学习模式
	  if( controlbuf.screenid == 0 && controlbuf.controlid == 9 && controlbuf.data == 1)
	  {
		  SysMode =1;
	  }
	  //返回分析模式
	  if( controlbuf.screenid == 1 && controlbuf.controlid == 9 && controlbuf.data == 1)
	  {
		  SysMode =0;
	  }

	  //分析模式保存信息
	  if(controlbuf.screenid == 1 && controlbuf.controlid >= 10 && controlbuf.controlid <= 16 )
	  {
		  if( controlbuf.data == 1 )
		  {
			  //计算出该波形对应的向�???





			  //保存相应信息
			  uint16_t eleNum = controlbuf.controlid - 10;
			  //存储时域信息
			  arm_copy_f32(dataTimNow,&dataTimSample[eleNum][0],128);
			  //存储频域信息
			  arm_copy_f32(dataFreqNow,&dataFreqSample[eleNum][0],10);
			  //存储参数
			  arm_copy_f32(dataParaNow,&dataParaSample[eleNum][0],20);


			  show_As_One_1(SysMode,2,&dataTimSample[eleNum][0],128);
			  //storageNum=2;


		  }
		  //点击取消则从flash内读取原来的数据
		  else
		  {
			  uint16_t eleNum = controlbuf.controlid - 10;
			  W25QXX_Read(&dataTimSample[eleNum], sectorBuf*4096 + eleNum*128*4, 128*4);
			  W25QXX_Read(&dataFreqSample[eleNum], (sectorBuf+1)*4096 +  eleNum * 10 * 4, 10*4);
			  W25QXX_Read(&dataParaSample[eleNum], (sectorBuf+2)*4096 +  eleNum * 20 * 4, 20*4);
			  show_As_One_1(SysMode,2,&dataTimSample[eleNum],128);
		  }
	  }
	  //分析模式点击确认学习
	  if(controlbuf.screenid == 1 && controlbuf.controlid == 17 && controlbuf.data == 1 )
	  {
		  uint32_t i,k;
		  tft_text(1, 20, "Please Wait!\0", 13);

		  //通过当前时域信息算出�???有组合坐�???
		  arm_fill_f32(0, dataVec, 128*2);   //清空坐标信息
		  //0下标为不接负载，记为0
		  dataVec[0][0][0]=0;
		  dataVec[0][0][1]=0;
		  dataVec[1][0][0]=0;
		  dataVec[1][0][1]=0;
		  dataVec[2][0][0]=0;
		  dataVec[2][0][1]=0;
		  dataVec[3][0][0]=0;
		  dataVec[3][0][1]=0;
		  dataVec[4][0][0]=0;
		  dataVec[4][0][1]=0;
		  //记录128个坐标信�???
		  for(i=1;i<128;i++)
		  {
			  for(k=0;k<7;k++)
			  {
				  if( ((uint8_t)i>>k) & 0x01 )
				  {
					  dataVec[0][i][0] += dataFreqSample[k][0];
					  dataVec[0][i][1] += dataFreqSample[k][1];

					  dataVec[1][i][0] += dataFreqSample[k][2];
					  dataVec[1][i][1] += dataFreqSample[k][3];

					  dataVec[2][i][0] += dataFreqSample[k][4];
					  dataVec[2][i][1] += dataFreqSample[k][5];

					  dataVec[3][i][0] += dataFreqSample[k][6];
					  dataVec[3][i][1] += dataFreqSample[k][7];

					  dataVec[4][i][0] += dataFreqSample[k][8];
					  dataVec[4][i][1] += dataFreqSample[k][9];
				  }
			  }
		  }

		  //存储�???有信�???

			W25QXX_Write(dataTimSample, sectorBuf*4096   ,  128*7*4);
			W25QXX_Write(dataFreqSample, (sectorBuf+1)*4096 , 10*7*4);
			W25QXX_Write(dataParaSample, (sectorBuf+2)*4096 , 20*7*4);
			W25QXX_Write(dataVec, (sectorBuf+3)*4096 , 5*128*2*4);
			tft_text(1, 20, "Finish!\0", 8);
			SysMode = 0;

	  }


  }
  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream7 global interrupt.
  */
void DMA1_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream7_IRQn 0 */

  /* USER CODE END DMA1_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi3_tx);
  /* USER CODE BEGIN DMA1_Stream7_IRQn 1 */

  /* USER CODE END DMA1_Stream7_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  /* USER CODE BEGIN TIM7_IRQn 1 */
	  __HAL_TIM_CLEAR_FLAG(&htim7, TIM_SR_UIF);
  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */
  WIFI_Receive_One();
  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
