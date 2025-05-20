/*
 * bsp_smg.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_SMG_H_
#define BSP_INC_BSP_SMG_H_
#include "main.h"

//#define TM1639_ADDR_GRID1_H           0xC0    // GRID4 高4位地址
//#define TM1639_ADDR_GRID1_L           0xC1    // GRID4 低4位地址
//
//#define TM1639_ADDR_GRID2_H           0xC2    // GRID5 高4位地址
//#define TM1639_ADDR_GRID2_L           0xC3    // GRID5 低4位地址
//
//#define TM1639_ADDR_GRID3_H           0xC4    // GRID6 高4位地址
//#define TM1639_ADDR_GRID3_L           0xC5    // GRID6 低4位地址




// 函数声明
void SMG_Display_Hour(void);  // 显示0-24小时时间

void SMG_Display_Err(uint8_t idata);

#endif /* BSP_INC_BSP_SMG_H_ */
