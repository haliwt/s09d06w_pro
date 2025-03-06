/*
 * bsp_streamlight.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_STREAMLIGHT_H_
#define BSP_STREAMLIGHT_H_
#include "main.h"

// TM1639 LED显示地址定义（GRID4-GRID8）
#define TM1639_ADDR_GRID4_H           0xC6    // GRID4 高4位地址
#define TM1639_ADDR_GRID4_L           0xC7    // GRID4 低4位地址

#define TM1639_ADDR_GRID5_H           0xC8    // GRID5 高4位地址
#define TM1639_ADDR_GRID5_L           0xC9    // GRID5 低4位地址

#define TM1639_ADDR_GRID6_H           0xCA    // GRID6 高4位地址
#define TM1639_ADDR_GRID6_L           0xCB    // GRID6 低4位地址

#define TM1639_ADDR_GRID7_H           0xCC    // GRID7 高4位地址
#define TM1639_ADDR_GRID7_L           0xCD    // GRID7 低4位地址

#define TM1639_ADDR_GRID8_H           0xCE    // GRID8 高4位地址
#define TM1639_ADDR_GRID8_L           0xCF    // GRID8 低4位地址

// LED测试结果
typedef enum {
    LED_TEST_OK = 0,    // 测试成功
    LED_TEST_ABORT      // 测试被中止
} LED_Test_Result_t;

// 函数声明
void StreamLight_Init(void);                          // 初始化流水灯
//void StreamLight_Set_Mode(StreamLight_Mode_t mode);   // 设置流水灯模式
void StreamLight_Update(void);                        // 更新流水灯状态
void StreamLight_Set_Speed(uint16_t speed_ms);        // 设置流水灯速度
void StreamLight_All_LED_Off(void);                   // 关闭所有40个LED
void Check_LED_Hardware_Test(void);                   // 检查并执行LED硬件测试
void StreamLight_Test_Init(void);                     // 初始化LED测试功能
uint8_t Is_LED_Testing(void);                        // 获取LED测试状态

#endif /* BSP_STREAMLIGHT_H_ */
