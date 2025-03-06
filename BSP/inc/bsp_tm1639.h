/*
 * bsp_tm1639.h
 *
 *  Created on: 2025年3月5日
 *      Author: Administrator
 */

#ifndef __BSP_TM1639_H_
#define __BSP_TM1639_H_
#include "main.h"

// TM1639 引脚定义
#define TM1639_STB_PIN                GPIO_PIN_11
#define TM1639_DIO_PIN                GPIO_PIN_10
#define TM1639_CLK_PIN                GPIO_PIN_12
#define TM1639_GPIO                   GPIOB

// TM1639 引脚操作宏定义
#define TM1639_CLK_SetHigh()          HAL_GPIO_WritePin(TM1639_GPIO, TM1639_CLK_PIN, GPIO_PIN_SET)
#define TM1639_CLK_SetLow()           HAL_GPIO_WritePin(TM1639_GPIO, TM1639_CLK_PIN, GPIO_PIN_RESET)
#define TM1639_DIO_SetHigh()          HAL_GPIO_WritePin(TM1639_GPIO, TM1639_DIO_PIN, GPIO_PIN_SET)
#define TM1639_DIO_SetLow()           HAL_GPIO_WritePin(TM1639_GPIO, TM1639_DIO_PIN, GPIO_PIN_RESET)
#define TM1639_STB_SetHigh()          HAL_GPIO_WritePin(TM1639_GPIO, TM1639_STB_PIN, GPIO_PIN_SET)
#define TM1639_STB_SetLow()           HAL_GPIO_WritePin(TM1639_GPIO, TM1639_STB_PIN, GPIO_PIN_RESET)

// TM1639 命令定义
#define TM1639_CMD_DATA               0x40    // 数据命令
#define TM1639_CMD_DISPLAY            0x88    // 显示控制命令

// TM1639 显示地址定义（每个数字需要高4位和低4位两个地址）
// 第1位数码管（最左）
#define TM1639_ADDR_DIG1_H           0xC0    // 高4位地址
#define TM1639_ADDR_DIG1_L           0xC1    // 低4位地址
// 第2位数码管（中间）
#define TM1639_ADDR_DIG2_H           0xC2    // 高4位地址
#define TM1639_ADDR_DIG2_L           0xC3    // 低4位地址
// 第3位数码管（最右）
#define TM1639_ADDR_DIG3_H           0xC4    //  高4位地址
#define TM1639_ADDR_DIG3_L           0xC5    // 低4位地址

// TM1639 显示模式
#define TM1639_DISPLAY_ON            0x08    // 显示开
#define TM1639_DISPLAY_OFF           0x00    // 显示关

// TM1639 亮度级别 (0-7)
#define TM1639_BRIGHTNESS_MAX        0x07    // 最大亮度
#define TM1639_BRIGHTNESS_MIN        0x00    // 最小亮度

// 特殊字符段码定义
//#define TM1639_CHAR_H               0x76    // H: 0111 0110 (b,c,e,f,g)
//#define TM1639_CHAR_DEGREE          0x63    // 度数符号 (b,c,g)
//#define TM1639_CHAR_C               0x39    // 摄氏度C (a,d,e,f)
//#define TM1639_CHAR_RH              0x50    // 湿度符号RH的H部分 (e,g)
//#define TM1639_DOT                  0x80    // 小数点 (dp)

#define SEG_A                       0x01
#define SEG_B                       0x02
#define SEG_C                       0x04
#define SEG_D                       0x08
#define SEG_E                       0x10
#define SEG_F                       0x20
#define SEG_G                       0x40
#define SEG_H                       0x80

// 函数声明
void TM1639_Init(void);                      // 初始化TM1639
void TM1639_Display_3_Digit(uint16_t num);   // 显示3位数字
void TM1639_Set_Brightness(uint8_t bright);  // 设置亮度
void TM1639_Display_ON_OFF(uint8_t status);  // 显示开关控制
void TM1639_Clear(void);                     // 清空显示
void TM1639_Display_H(uint8_t position);     // 在指定位置显示字母H
void TM1639_Display_Temperature(int8_t temp); // 显示温度值（-9到99℃）
void TM1639_Display_Humidity(uint8_t humi);  // 显示湿度值（0-99%RH）
void TM1639_Display_Decimal(uint16_t num, uint8_t dot_pos); // 显示带小数点的数字
void TM1639_Write_Digit_Full(uint8_t addr_h, uint8_t addr_l, uint8_t data); // 写入完整的一位数码管
void TM1639_All_Off(void);                  // 关闭所有显示（包括数码管和LED）

#endif /* __BSP_TM1639_H_ */
