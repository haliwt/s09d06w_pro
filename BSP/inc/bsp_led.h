/*
 * bsp_led.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "main.h"


#define LED_TAPE_CTL_ON()        HAL_GPIO_WritePin(LED_TAPE_CTL_GPIO_Port, LED_TAPE_CTL_Pin,GPIO_PIN_SET)
#define LED_TAPE_CTL_OFF()       HAL_GPIO_WritePin(LED_TAPE_CTL_GPIO_Port, LED_TAPE_CTL_Pin,GPIO_PIN_RESET)

#define LED_POWER_ON()           HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin, GPIO_PIN_RESET)
#define LED_POWER_OFF()          HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin, GPIO_PIN_SET)

#define LED_AI_ON()              HAL_GPIO_WritePin(LED_AI_GPIO_Port, LED_AI_Pin, GPIO_PIN_RESET)
#define LED_AI_OFF()             HAL_GPIO_WritePin(LED_AI_GPIO_Port, LED_AI_Pin, GPIO_PIN_SET)

#define LED_WIFI_ON()            HAL_GPIO_WritePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin, GPIO_PIN_RESET)
#define LED_WIFI_OFF()           HAL_GPIO_WritePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin, GPIO_PIN_SET)

#define LED_TEMP_SINGLE_ON()            HAL_GPIO_WritePin(LED_TEMP_SYMBOL_GPIO_Port, LED_TEMP_SYMBOL_Pin, GPIO_PIN_RESET)
#define LED_TEMP_SINGLE_OFF()           HAL_GPIO_WritePin(LED_TEMP_SYMBOL_GPIO_Port, LED_TEMP_SYMBOL_Pin, GPIO_PIN_SET)

#define LED_HUM_SINGLE_ON()      HAL_GPIO_WritePin(LED_HUM_SYMBOL_GPIO_Port, LED_HUM_SYMBOL_Pin, GPIO_PIN_RESET)
#define LED_HUM_SINGLE_OFF()     HAL_GPIO_WritePin(LED_HUM_SYMBOL_GPIO_Port, LED_HUM_SYMBOL_Pin, GPIO_PIN_SET)

#define LED_DRY_ON()             HAL_GPIO_WritePin(LED_DRY_GPIO_Port, LED_DRY_Pin, GPIO_PIN_RESET)
#define LED_DRY_OFF()            HAL_GPIO_WritePin(LED_DRY_GPIO_Port, LED_DRY_Pin, GPIO_PIN_SET)

#define LED_PLASMA_ON()          HAL_GPIO_WritePin(LED_PLASMA_GPIO_Port, LED_PLASMA_Pin, GPIO_PIN_RESET)
#define LED_PLASMA_OFF()         HAL_GPIO_WritePin(LED_PLASMA_GPIO_Port, LED_PLASMA_Pin, GPIO_PIN_SET)

#define LED_MOUSE_ON()           HAL_GPIO_WritePin(LED_MOUSE_GPIO_Port, LED_MOUSE_Pin, GPIO_PIN_RESET)
#define LED_MOUSE_OFF()          HAL_GPIO_WritePin(LED_MOUSE_GPIO_Port, LED_MOUSE_Pin, GPIO_PIN_SET)

void power_on_led(void);
void power_off_led(void);
void LED_Power_Breathing(void);   // LED呼吸灯函数

void LED_FUN_ON(void);




#endif /* BSP_INC_BSP_LED_H_ */
