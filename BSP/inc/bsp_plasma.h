/*
 * bsp_plasma.h
 *
 *  Created on: 2025年3月7日
 *      Author: Administrator
 */

#ifndef _BSP_PLASMA_H_
#define _BSP_PLASMA_H_

#include "main.h"

#define PLASMA_OPEN()   do{PLASMA_GPIO_Port->BSRR = PLASMA_Pin;}while(0)
#define PLASMA_CLOSE()  do{PLASMA_GPIO_Port->BSRR |= (uint32_t)PLASMA_Pin<<16;}while(0)
    



#endif /* BSP_INC_BSP_PLASMA_H_ */
