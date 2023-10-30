/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define sysled_Pin GPIO_PIN_13
#define sysled_GPIO_Port GPIOC
#define inConf_Pin GPIO_PIN_0
#define inConf_GPIO_Port GPIOA
#define relay1_Pin GPIO_PIN_1
#define relay1_GPIO_Port GPIOA
#define relay2_Pin GPIO_PIN_2
#define relay2_GPIO_Port GPIOA
#define relay3_Pin GPIO_PIN_3
#define relay3_GPIO_Port GPIOA
#define relay4_Pin GPIO_PIN_4
#define relay4_GPIO_Port GPIOA
#define relay5_Pin GPIO_PIN_5
#define relay5_GPIO_Port GPIOA
#define relay6_Pin GPIO_PIN_6
#define relay6_GPIO_Port GPIOA
#define input1_Pin GPIO_PIN_7
#define input1_GPIO_Port GPIOA
#define input3_Pin GPIO_PIN_0
#define input3_GPIO_Port GPIOB
#define input4_Pin GPIO_PIN_1
#define input4_GPIO_Port GPIOB
#define input2_Pin GPIO_PIN_8
#define input2_GPIO_Port GPIOA
#define input5_Pin GPIO_PIN_3
#define input5_GPIO_Port GPIOB
#define input6_Pin GPIO_PIN_4
#define input6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
