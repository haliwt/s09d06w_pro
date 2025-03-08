/*
 * bsp_fan.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_FAN_H_
#define BSP_INC_BSP_FAN_H_
#include "main.h"

#define FAN_COM_SetHigh()         do{FAN_CCW_GPIO_Port->BSRR |=FAN_CCW_Pin;}while(0)//HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_SET)    // output high level
#define FAN_COM_SetLow()          do{FAN_CCW_GPIO_Port->BSRR |=(uint32_t)FAN_CCW_Pin<<16;}while(0)//HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_RESET)    // output low level

#define FAN_CCW_SetHigh()         do{FAN_COM_GPIO_Port->BSRR |=FAN_COM_Pin;}while(0)//HAL_GPIO_WritePin(FAN_GPIO,FAN_COM_GPIO_Port-)    // output high level
#define FAN_CCW_SetLow()          do{FAN_COM_GPIO_Port->BSRR |=(uint32_t)FAN_COM_Pin<<16;}while(0)    // output low level

//#define FAN_CCW_SetHigh()         do{FAN_CCW_GPIO_Port->BSRR |=FAN_CCW_Pin;}while(0)//HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_SET)    // output high level
//#define FAN_CCW_SetLow()          do{FAN_CCW_GPIO_Port->BSRR |=(uint32_t)FAN_CCW_Pin<<16;}while(0)//HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_RESET)    // output low level


void FAN_Stop(void);

void Fan_Full_Speed(void);





void fan_run_handler(void);



#endif /* BSP_INC_BSP_FAN_H_ */
