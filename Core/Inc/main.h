/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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

#define LED_TEMP_SYMBOL_Pin 					GPIO_PIN_8
#define LED_TEMP_SYMBOL_GPIO_Port 				GPIOB


#define LED_HUM_SYMBOL_Pin 					GPIO_PIN_13
#define LED_HUM_SYMBOL_GPIO_Port 			GPIOC

#define LED_POWER_Pin 						GPIO_PIN_14
#define LED_POWER_GPIO_Port 				GPIOC

#define LED_WIFI_Pin 					GPIO_PIN_3
#define LED_WIFI_GPIO_Port 				GPIOB

#define LED_AI_Pin 						GPIO_PIN_4
#define LED_AI_GPIO_Port 				GPIOB

#define LED_DRY_Pin 					GPIO_PIN_5
#define LED_DRY_GPIO_Port 				GPIOB

#define LED_PLASMA_Pin 					GPIO_PIN_6
#define LED_PLASMA_GPIO_Port 			GPIOB

#define LED_MOUSE_Pin 					GPIO_PIN_7
#define LED_MOUSE_GPIO_Port 			GPIOB



//GPIO CTL

#define RELAY_Pin                 GPIO_PIN_15
#define RELAY_GPIO_Port           GPIOC

#define FAN_COM_Pin GPIO_PIN_5
#define FAN_COM_GPIO_Port GPIOA

#define FAN_PWM_Pin GPIO_PIN_6
#define FAN_PWM_GPIO_Port GPIOA

#define FAN_CCW_Pin GPIO_PIN_7
#define FAN_CCW_GPIO_Port GPIOA

#define PLASMA_Pin GPIO_PIN_15
#define PLASMA_GPIO_Port GPIOA
//BUZZER GPIO
#define BUZZER_PWM_Pin GPIO_PIN_1
#define BUZZER_PWM_GPIO_Port GPIOB
//TM1639 GPIO
#define MCU_DIO_Pin 						GPIO_PIN_10
#define MCU_DIO_GPIO_Port                  GPIOB

#define MCU_STB_Pin 						GPIO_PIN_11
#define MCU_STB_GPIO_Port 					GPIOB

#define MCU_CLK_Pin 						GPIO_PIN_12
#define MCU_CLK_GPIO_Port 					GPIOB

//PWM CTL
#define ULTRA_PWM_Pin GPIO_PIN_8
#define ULTRA_PWM_GPIO_Port GPIOA

//TAPE LED CTL
#define LED_TAPE_CTL_Pin 				GPIO_PIN_11
#define LED_TAPE_CTL_GPIO_Port 			GPIOA

//KEY GPIO
#define KEY_POWER_Pin GPIO_PIN_0
#define KEY_POWER_GPIO_Port GPIOD
#define KEY_POWER_EXTI_IRQn EXTI0_1_IRQn

#define KEY_MODE_Pin GPIO_PIN_1
#define KEY_MODE_GPIO_Port GPIOD
#define KEY_MODE_EXTI_IRQn EXTI0_1_IRQn

#define KEY_UP_Pin GPIO_PIN_2
#define KEY_UP_GPIO_Port GPIOD
#define KEY_UP_EXTI_IRQn EXTI2_3_IRQn

#define KEY_DOWN_Pin GPIO_PIN_3
#define KEY_DOWN_GPIO_Port GPIOD
#define KEY_DOWN_EXTI_IRQn EXTI2_3_IRQn

//DHT11 GPIO
#define TEMP_SENSOR_Pin 					GPIO_PIN_4
#define TEMP_SENSOR_GPIO_Port 				GPIOA


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
