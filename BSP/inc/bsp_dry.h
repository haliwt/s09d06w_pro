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





#endif /* BSP_INC_BSP_DRY_H_ */
