/*
 * bsp_power.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

#define SWITCH_THRESHOLD 2




typedef struct{

  uint8_t process_on_step;
  uint8_t process_off_step;


}POWER_RUN_STATE;

POWER_RUN_STATE gl_run;

typedef enum {
    DISPLAY_TEMP = 0,
    DISPLAY_HUM = 1,
	DISPLAY_TIME = 2
} DisplayMode;

//DisplayMode disp_temp_hum = DISPLAY_TEMP;  // 默认显示温度
uint8_t disp_temp_hum;

/**********************************************************************
	*
	*Functin Name: void power_on_init_ref(void)
	*Function : 
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_onoff_handler(uint8_t data)
{
  static uint8_t power_on_flag;
   switch(data){	

	   case power_on :

		// 检查LED硬件测试
		Check_LED_Hardware_Test();

        // 只有在不进行LED测试时才执行正常的power_on处理
        if(!Is_LED_Testing())
        {
            power_on_run_handler();
            link_wifi_to_tencent_handler(g_wifi.wifi_led_fast_blink_flag); //detected ADC of value 
            works_run_two_hours_state();
            set_temperature_value_handler();
        break;

	  case power_off:
  
         if(power_on_flag==0){
             power_on_flag ++;
			 buzzer_sound();
		 }
         power_off_run_handler();

	   break;
  
         }
	  }



}


/**********************************************************************
	*
	*Functin Name: void power_on_init_ref(void)
	*Function : 
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_on_init_ref(void)
{
	       g_pro.gAI =1;
		   g_pro.gDry =1;
		   g_pro.gPlasma =1;
		   g_pro.gMouse = 1;
		   g_pro.gTemp_value = 40;
		   //display time timing value 
		   g_pro.gdisp_hours_value =0;
		   g_pro.gdisp_timer_hours_value =0; //设置定时时间，
		   g_pro.gset_temperture_flag =0;
		   g_pro.gtime_timer_define_flag=normal_time_mode; //
		   
		   // function led is turn on 
             power_on_led();
		   //display smg led turn on
		    Fan_Full_Speed();
		    DHT11_Display_Data(0); //display temperature value 
		    
		   
           //timer 
		   g_pro.gTimer_disp_time_second= 0;
	       g_pro.gTimer_timer_time_second=0;
		   g_wifi.set_wind_speed_value = 100;


}


/**********************************************************************
	*
	*Functin Name: power_on_run_handler(void)
	*Function : 
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_on_run_handler(void)
{
 
	
	switch(gl_run.process_on_step){


     case 0:  //initial reference 
       gl_run.process_off_step =0 ; //clear power off process step .
       if(g_wifi.gwifi_link_net_state_flag == wifi_no_link){
	      
		   power_on_init_ref();
       }
	   else{ //has wifi net initial
		   smartphone_timer_power_handler();

	   }

	   g_pro.gTimer_two_hours_counter = 0;
	   gl_run.process_on_step =1;
	 break;

	 case 1:

	 if (g_pro.gtime_timer_define_flag == timer_time_mode) {
		// 如果计时器超过阈值，切换显示模式
		if (g_pro.gTimer_switch_temp_hum > SWITCH_THRESHOLD) {
			g_pro.gTimer_switch_temp_hum = 0; // 重置计时器
	
			disp_temp_hum++;
			if (disp_temp_hum > 3) {
				disp_temp_hum = 0; // 循环显示状态
			}
	
			// 根据状态调用显示函数
			switch (disp_temp_hum) {
				case 1:
					DHT11_Display_Data(DISPLAY_TEMP); // 显示温度
					break;
				case 2:
					DHT11_Display_Data(DISPLAY_HUM);  // 显示湿度
					break;
				case 3:
					LED_AI_OFF();
					TM1639_Display_3_Digit(g_pro.gdisp_hours_value); // 显示时间
					break;
			}
		}
	} 
	else {
		// 如果计时器超过阈值，切换布尔显示状态
  
	if(g_pro.gtime_timer_define_flag == normal_time_mode && g_pro.gset_temperture_flag != 1){ //正常模式
		if (g_pro.gTimer_switch_temp_hum > SWITCH_THRESHOLD) {
			g_pro.gTimer_switch_temp_hum = 0; // 重置计时器
	       
			disp_temp_hum = disp_temp_hum ^ 0x01;   // 切换布尔状态
			DHT11_Display_Data(disp_temp_hum); // 显示温度或湿度
			
		}
	}

	  gl_run.process_on_step =2;

	 break;

	 case 2: //WIFI link process
	 
       if(g_key.key_long_power_flag !=  KEY_LONG_POWER && g_wifi.gwifi_link_net_state_flag ==0){

             wifi_led_slowly_blink();
       	}

	   

	   

	   gl_run.process_on_step =3;

	 break;

	 case 3: // wifi function

	    gl_run.process_on_step =1;

	 break;

	}
   
     }
}
/**********************************************************************
	*
	*Functin Name: void power_off_run_handler(void)
	*Function :
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_off_run_handler(void)
{

   static uint8_t fan_run_one_minute , fan_flag;
   switch(gl_run.process_off_step){

   case 0:
   	  gl_run.process_on_step =0;

   	  power_off_led();
      TM1639_Display_ON_OFF(0);
	  g_key.key_long_power_flag  = 0;
	  g_key.key_long_mode_flag = 0;
	  g_pro.gset_temperture_flag =0;   //set temperature flag 
	  g_pro.gtime_timer_define_flag = normal_time_mode;

	  fan_run_one_minute = 1;
	  g_pro.gTimer_fan_run_one_minute =0;

      gl_run.process_off_step = 1;
	  

   break;

   case 1:

     if(fan_flag == 0){
	 	fan_flag++;
	  fan_run_one_minute =2;
     }

	 if(fan_run_one_minute ==1){

	     if(g_pro.gTimer_fan_run_one_minute  < 61){

              Fan_Full_Speed();
		 }
		 else{

		    fan_run_one_minute++;
			FAN_Stop();

		 }

	 }

     mainboard_close_all_fun();

     LED_Power_Breathing();

   

   break;

   
   	}

}



