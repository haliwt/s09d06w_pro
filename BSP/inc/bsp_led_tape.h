#ifndef __BSP_LED_TAPE_H_
#define __BSP_LED_TAPE_H_
#include "main.h"


#define LED_TAPE_CTL_ON()        do{LED_TAPE_CTL_GPIO_Port->BSRR |=LED_TAPE_CTL_Pin;}while(0)
#define LED_TAPE_CTL_OFF()       do{LED_TAPE_CTL_GPIO_Port->BSRR |=(uint32_t)LED_TAPE_CTL_Pin<<16;}while(0)




#endif 

