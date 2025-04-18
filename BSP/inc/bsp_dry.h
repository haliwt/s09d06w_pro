/*
 * bsp_dry.h
 *
 *  Created on: 2025年3月7日
 *      Author: Administrator
 */

#ifndef __BSP_DRY_H_
#define __BSP_DRY_H_
#include "main.h"

#define DRY_OPEN()    do{RELAY_GPIO_Port->BSRR |= RELAY_Pin;}while(0)
#define DRY_CLOSE()   do{RELAY_GPIO_Port->BSRR |=(uint32_t)RELAY_Pin<<16;}while(0)


#define LED_DRY_ON()       do{LED_DRY_GPIO_Port->BSRR |=(uint32_t)LED_DRY_Pin << 16;}while(0) //sys_write_gpio_pin_value(LED_DRY_GPIO_Port, LED_DRY_Pin, GPIO_PIN_RESET)
#define LED_DRY_OFF()      do{LED_DRY_GPIO_Port->BSRR |=LED_DRY_Pin;}while(0)//sys_write_gpio_pin_value(LED_DRY_GPIO_Port, LED_DRY_Pin, GPIO_PIN_SET)






#endif /* BSP_INC_BSP_DRY_H_ */
