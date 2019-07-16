/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define CH0_VIN_Pin GPIO_PIN_0
#define CH0_VIN_GPIO_Port GPIOA
#define CH1_IOUT_Pin GPIO_PIN_1
#define CH1_IOUT_GPIO_Port GPIOA
#define CH2_VLED_Pin GPIO_PIN_2
#define CH2_VLED_GPIO_Port GPIOA
#define CH3_TBD_Pin GPIO_PIN_3
#define CH3_TBD_GPIO_Port GPIOA
#define CH4_TR_Pin GPIO_PIN_4
#define CH4_TR_GPIO_Port GPIOA
#define CH5_TG_Pin GPIO_PIN_5
#define CH5_TG_GPIO_Port GPIOA
#define CH6_TB1_Pin GPIO_PIN_6
#define CH6_TB1_GPIO_Port GPIOA
#define CH7_TB2_Pin GPIO_PIN_7
#define CH7_TB2_GPIO_Port GPIOA
#define CH8_LR_Pin GPIO_PIN_0
#define CH8_LR_GPIO_Port GPIOB
#define CH9_LB_Pin GPIO_PIN_1
#define CH9_LB_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define TXD1_Pin GPIO_PIN_9
#define TXD1_GPIO_Port GPIOA
#define RXD1_Pin GPIO_PIN_10
#define RXD1_GPIO_Port GPIOA
#define T2CH2_R_Pin GPIO_PIN_3
#define T2CH2_R_GPIO_Port GPIOB
#define T3CH1_G_Pin GPIO_PIN_4
#define T3CH1_G_GPIO_Port GPIOB
#define T3CH2_B_Pin GPIO_PIN_5
#define T3CH2_B_GPIO_Port GPIOB
#define ACOK_Pin GPIO_PIN_9
#define ACOK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
