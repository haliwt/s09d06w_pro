/*
 * bsp_smg.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#include "bsp.h"

#define TM1639_CHAR_H               0x76    // H: 0111 0110 (b,c,e,f,g)


static const uint8_t Number_Table[] = {
    0xF3, // 0: 0011 1111   （f,e,d,c,b,a）--0x3F
    0x60, // 1: 0000 0110 --0x06--写数据式冲低位开始，向高位开始写
    0xB5, // 2: 0101 1011 --0x5B
    0xF4, // 3: 0100 1111 --0x4F
    0x66, // 4: 0110 0110
    0xD6, // 5: 0110 1101 --0x6D
    0xD7, // 6: 0111 1101  --0x7D 
    0x70, // 7: 0000 0111
    0xF7, // 8: 0111 1111
    0xF6  // 9: 0110 1111
};


// 字母和特殊字符显示码
//static const uint8_t TM1639_Char_Table[] = {
//    0x67, // H: 0111 0110 (b,c,e,f,g)
//    0x36, // °: 0110 0011 (b,c,g)
//    0x93, // C: 0011 1001 (a,d,e,f)
//    0x05  // RH的H部分: 0101 0000 (e,g)
//};


// 字母和特殊字符显示码
static const uint8_t TM1639_Char_Err_Table[] = {
    0x97, // E: 0111 1001 (b,c,e,f,g)
    0x05, // r: 0101 0000 (b,c,g)
    
  
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


/******************************************************************************
	*
	*Function Name:void SMG_Display_Err(uint8_t idata)
	*Funcion: 
	*Input Ref: idata: 1 -ptc warning  2 - fan warning
	*Return Ref:
	*
******************************************************************************/
void SMG_Display_Err(uint8_t idata)
{

	 TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Char_Err_Table[0]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Char_Err_Table[1]);
        
    if(idata==1){ //ptc warning 
	// 写入个位（最右边）'H'
     TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,Number_Table[1]);

    }
	else if(idata == 2){
		
	  TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,Number_Table[2]);
    }
	else{
	   TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Char_Err_Table[1]);

	}


}

