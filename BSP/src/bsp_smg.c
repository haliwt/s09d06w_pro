/*
 * bsp_smg.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#include "bsp.h"

#define TM1639_CHAR_H               0x76    // H: 0111 0110 (b,c,e,f,g)


static const uint8_t Number_Table[] = {
    0x3F, // 0: 0011 1111   （f,e,d,c,b,a）
    0x06, // 1: 0000 0110
    0x5B, // 2: 0101 1011
    0x4F, // 3: 0100 1111
    0x66, // 4: 0110 0110
    0x6D, // 5: 0110 1101
    0x7D, // 6: 0111 1101
    0x07, // 7: 0000 0111
    0x7F, // 8: 0111 1111
    0x6F  // 9: 0110 1111
};


/**
 * @brief  显示0-24小时时间
 * @param  None
 * @retval None
 * @note   前两位显示小时(00-24)，最后一位显示'H'
 */
void SMG_Display_Hour(void)
{
    uint8_t hour;
    
    // 计算小时数 (3600秒=1小时)
    hour = g_pro.gTimer_disp_time_second / 3600;
    
    // 限制在0-24小时范围内
    if(hour > 24) hour = 24;
    
    // 显示十位数字（始终显示，小于10时显示0）
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 
        Number_Table[hour / 10]);
    
    // 显示个位数字
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L,
        Number_Table[hour % 10]);
    
    // 最后一位显示'H'
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_CHAR_H);
}
