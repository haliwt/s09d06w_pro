/*
 * bsp_fan.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"




#define FAN_PWM_100     40

#define FAN_PWM_90      36

#define FAN_PWM_80      32


/********************************************************
*
*Function Name:void SetLevel_Fan_PWMA(uint8_t levelval)
*Function: 
*
*
********************************************************/
static void SetLevel_Fan_PWMA(uint8_t levelval)
{
	 MX_TIM16_Init(levelval);
	 HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
	
}


void FAN_Stop(void)
{
   FAN_COM_SetLow(); //brake
   FAN_CCW_SetLow();//SetLevel_Fan_PWMA(0);//SetLevel_Fan_PWMA(16);
   SetLevel_Fan_PWMA(0);
}



void Fan_One_Speed(void)
{
   static uint8_t one_speed=0xff;
	 FAN_COM_SetLow();
     FAN_CCW_SetHigh();
     if(one_speed != g_pro.g_fan_switch_gears_flag){
        g_pro.g_fan_switch_gears_flag++;
        one_speed = g_pro.g_fan_switch_gears_flag ;  //one_speed =2,5,8
	    SetLevel_Fan_PWMA(FAN_PWM_80);

     }


}

void Fan_Two_Speed(void)
{
     static uint8_t two_speed=0xff;
      FAN_COM_SetLow();
     FAN_CCW_SetHigh();
      if(two_speed != g_pro.g_fan_switch_gears_flag){
         g_pro.g_fan_switch_gears_flag++;
         two_speed = g_pro.g_fan_switch_gears_flag;  //two_speed = 3;6,9
	     SetLevel_Fan_PWMA(FAN_PWM_90);
       }

}

 void Fan_Full_Speed(void)
{
   static uint8_t full_speed = 0xff;
     FAN_COM_SetLow();
     FAN_CCW_SetHigh();
    if(full_speed != g_pro.g_fan_switch_gears_flag){
        g_pro.g_fan_switch_gears_flag++;
         full_speed = g_pro.g_fan_switch_gears_flag;  //full_speed =1,4,7,10
         SetLevel_Fan_PWMA(FAN_PWM_100);
    }

}



void Fan_RunSpeed_Fun(void)
{

    if(g_wifi.set_wind_speed_value < 34 ){
        Fan_One_Speed();
    }
    else if(g_wifi.set_wind_speed_value > 33  && g_wifi.set_wind_speed_value < 67 ){

        Fan_Two_Speed();

    }
    else if(g_wifi.set_wind_speed_value > 66){

         Fan_Full_Speed();

    }

      

}





