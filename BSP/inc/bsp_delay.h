/*
 * bsp_delay.h
 *
 *  Created on: 2025年3月5日
 *      Author: Administrator
 */

#ifndef __BSP_DELAY_H_
#define __BSP_DELAY_H_
#include "main.h"




void delay_init(uint16_t sysclk);   /* 初始化延迟函数 */
void delay_ms(uint16_t nms);        /* 延时nms */
void delay_us(uint32_t nus);        /* 延时nus */



#endif /* BSP_INC_BSP_DELAY_H_ */
