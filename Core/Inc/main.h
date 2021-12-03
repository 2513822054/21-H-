/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "Zworkman.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DAC8563_CLR_Pin GPIO_PIN_2
#define DAC8563_CLR_GPIO_Port GPIOE
#define DAC8563_SYN_Pin GPIO_PIN_4
#define DAC8563_SYN_GPIO_Port GPIOE
#define DAC8563_SCK_Pin GPIO_PIN_6
#define DAC8563_SCK_GPIO_Port GPIOE
#define DAC8563_DIN_Pin GPIO_PIN_0
#define DAC8563_DIN_GPIO_Port GPIOF
#define DAC8563_LD_Pin GPIO_PIN_2
#define DAC8563_LD_GPIO_Port GPIOF
#define AD9959_SD0_Pin GPIO_PIN_7
#define AD9959_SD0_GPIO_Port GPIOE
#define AD9959_SCK_Pin GPIO_PIN_9
#define AD9959_SCK_GPIO_Port GPIOE
#define AD9959_CS_Pin GPIO_PIN_11
#define AD9959_CS_GPIO_Port GPIOE
#define AD9959_UP_Pin GPIO_PIN_13
#define AD9959_UP_GPIO_Port GPIOE
#define AD9959_RST_Pin GPIO_PIN_15
#define AD9959_RST_GPIO_Port GPIOE
#define SPI1_CS_Pin GPIO_PIN_8
#define SPI1_CS_GPIO_Port GPIOG
#define SPI3_CS_Pin GPIO_PIN_15
#define SPI3_CS_GPIO_Port GPIOA
#define ADS8688_daisi_Pin GPIO_PIN_0
#define ADS8688_daisi_GPIO_Port GPIOD
#define ADS8688_HIGH_Pin GPIO_PIN_1
#define ADS8688_HIGH_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOG
#define triggerInput_Pin GPIO_PIN_0
#define triggerInput_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
