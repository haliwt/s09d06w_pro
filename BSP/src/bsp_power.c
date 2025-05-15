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

	
		power_on_run_handler();
        if(gl_run.process_on_step !=0){ //logically rigorous
		link_wifi_to_tencent_handler(g_wifi.wifi_led_fast_blink_flag); //detected ADC of value 
		
		set_temperature_value_handler();
		set_timer_timing_value_handler();

		wifi_led_fast_blink();
		works_run_two_hours_state();

        }
			
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

		   g_pro.key_gtime_timer_define_flag=normal_time_mode; //
		   g_pro.g_disp_timer_or_temp_flag = normal_time_mode;
		
		 
		   // function led is turn on 
            power_on_led();
		   //display smg led turn on
		    Fan_Full_Speed();
		    DHT11_Display_Data(0); //display temperature value 
		    DRY_OPEN();
			PLASMA_OPEN();
			mouse_open();
		   
           //timer 
           g_pro.g_manual_shutoff_dry_flag = 0;
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

   static uint8_t read_error_flag,switch_adc,switch_dht11;

	switch(gl_run.process_on_step){


     case 0:  //initial reference 
       gl_run.process_off_step =0 ; //clear power off process step .

	   if(g_disp.g_second_disp_flag == 1){
	   	  SendData_Set_Command(CMD_POWER,open);
		  osDelay(5);
     
		  Update_DHT11_ToDisplayBoard_Value();
		  osDelay(5);
	   
	   }
      
       if(g_wifi.gwifi_link_net_state_flag == wifi_no_link || g_wifi.app_timer_power_on_flag == 0){
	      
		   power_on_init_ref();

       }
	   else if(g_wifi.gwifi_link_net_state_flag == wifi_link_success &&  g_wifi.app_timer_power_on_flag == 0){ //has wifi net initial
		  
		   power_on_init_ref();
		
       	
		  MqttData_Publish_SetOpen(1);  
		  osDelay(50);//HAL_Delay(200);
		  Update_DHT11_ToDisplayBoard_Value();
		  osDelay(50);//HAL_Delay(200);
	        
		
		   MqttData_Publish_Update_Data();
		   osDelay(50);//HAL_Delay(200);
	   }
	   else{

		  smartphone_timer_power_handler();

	   }
	   key_referen_init();
	   
	 
	   g_pro.gTimer_send_dht11_disp=5;
       
	   g_pro.gTimer_two_hours_counter = 0;
	   g_pro.g_fan_switch_gears_flag++;
	   gl_run.process_off_step=0;
	   
	   g_wifi.wifi_led_fast_blink_flag=0;
	   g_pro.key_set_temperature_flag=0;
	   g_pro.works_two_hours_interval_flag=0; //WT.EDIT 2025.05.07

	   gl_run.process_on_step =1;
	 break;

	 case 1:


	  if(g_disp.g_second_disp_flag == 1){
         
	    if(g_pro.gTimer_send_dht11_disp > 2){ //3s
	       g_pro.gTimer_send_dht11_disp=0;
           Update_DHT11_ToDisplayBoard_Value();

	   }
	  }
	   gl_run.process_on_step =2;


	case 2:

	
      if(g_pro.key_set_temperature_flag==1){

         read_error_flag= DHT11_Display_Data(DISPLAY_TEMP); // 显示温度
         if(read_error_flag == 0)DHT11_Display_Data(DISPLAY_TEMP);

      }
      else if(g_key.mode_key_switch_time_mode == timer_time_mode){

    	 if(g_pro.gTimer_switch_set_timer_times < 4){
    	           g_pro.gAI=0;
    	 		   LED_AI_OFF();
		           HUMIDITY_ICON_OFF();
		           TEMP_ICON_OFF();//WT.EDIT 2025.04.28
    	 		   TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value);//WT.EDIT 2025.04.23
    	  }
    	 else{
    		 if(g_pro.g_disp_timer_or_temp_flag == normal_time_mode){
    			 g_pro.gAI=1;
    			 LED_AI_ON();
    			 TEMP_ICON_ON();//WT.EDIT 2025.04.28
    			 read_error_flag=DHT11_Display_Data(0); // 显示温度
    			 if(read_error_flag==0)DHT11_Display_Data(0); 

    		 }

    		  g_key.mode_key_switch_time_mode=input_normal_null;

    	 }


      }
      else if((g_pro.g_disp_timer_or_temp_flag == timer_time_mode) && read_wifi_temperature_value()==0){
		// 如果计时器超过阈值，切换显示模式

		  if (g_pro.gTimer_switch_temp_hum > SWITCH_THRESHOLD) {
			g_pro.gTimer_switch_temp_hum = 0; // 重置计时器
	
			disp_temp_hum++;
			if (disp_temp_hum > 3) {
				disp_temp_hum = 1; // 循环显示状态
			}
	
			// 根据状态调用显示函数
			switch (disp_temp_hum) {
				case 1:
					LED_TEMP_SINGLE_ON();
					LED_HUM_SINGLE_OFF();

					read_error_flag =DHT11_Display_Data(DISPLAY_TEMP); // 显示温度
					if(read_error_flag == 0)DHT11_Display_Data(DISPLAY_TEMP); // 显示温度
					break;
				case 2:
                    LED_TEMP_SINGLE_OFF();
					LED_HUM_SINGLE_ON();
				    read_error_flag =DHT11_Display_Data(DISPLAY_HUM);  // 显示湿度
					if(read_error_flag == 0)DHT11_Display_Data(DISPLAY_HUM);  // 显示湿度
					break;
				case 3:
					LED_AI_OFF();
					LED_TEMP_SINGLE_OFF();
					LED_HUM_SINGLE_OFF();
					TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value); // 显示时间
					break;
			}
		}
	} 
	else {
		// 如果计时器超过阈值，切换布尔显示状态,不显示时间
       if((g_pro.g_disp_timer_or_temp_flag == normal_time_mode) && read_key_up_down_mode()!=1 && read_wifi_temperature_value()==0){ //正常模式

			   g_pro.gAI=1;
			   LED_AI_ON();
               if (g_pro.gTimer_switch_temp_hum > SWITCH_THRESHOLD ){
				g_pro.gTimer_switch_temp_hum = 0; // 重置计时器
		        if(disp_temp_hum > 1)disp_temp_hum=0;
				disp_temp_hum = disp_temp_hum ^ 0x01;   // 切换布尔状态
				read_error_flag= DHT11_Display_Data(disp_temp_hum); // 显示温度或湿度
				if(read_error_flag == 0)DHT11_Display_Data(disp_temp_hum); // 显示温度或湿度
				
			 }
		 	}
       	}

	  gl_run.process_on_step =3;

	 break;

	 case 3: //WIFI link process
	 
   
		if(g_wifi.gTimer_update_dht11_data > 7 && (g_wifi.gwifi_link_net_state_flag ==1 || g_wifi.gwifi_link_net_state_flag ==1)){
			   g_wifi.gTimer_update_dht11_data=0;

			   if(g_wifi.gwifi_link_net_state_flag ==1){

			       switch_dht11 = switch_dht11 ^0x01;
				   if(switch_dht11==1){
		         	   Subscriber_Data_FromCloud_Handler();
				
	                  osDelay(30);
				   	}
				    else{
					Update_Dht11_Totencent_Value()	;
					osDelay(20);


					}
				   
			   	}

            
				if(g_disp.g_second_disp_flag == 1){                     
					 sendData_Real_TimeHum(g_pro.g_humidity_value, g_pro.g_temperature_value);
					 osDelay(5);

				}
		    }
		

	   gl_run.process_on_step =4;

	 break;

	 case 4: // wifi function
         if(g_pro.gTimer_display_adc_value > 3){
		 	g_pro.gTimer_display_adc_value=0;

		    switch_adc = switch_adc ^ 0x01;
		    if(switch_adc==1){
               Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,5);
			}
			else{
				

				if(g_pro.works_two_hours_interval_flag==0){
	                Get_Fan_Adc_Fun(ADC_CHANNEL_0,5);

		        }

            }

         }
		 
		

		 
		    
	    gl_run.process_on_step =1;

	 break;

	 default :

	  break;

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

   static uint8_t fan_run_one_minute,fan_flag,wifi_first_connect;
   switch(gl_run.process_off_step){

   case 0:
   	  gl_run.process_on_step =0;
   	  g_disp.g_second_disp_flag=1;
   	  power_off_led();
      TM1639_Display_ON_OFF(0);
	  g_key.key_long_power_flag  = 0;
	  g_key.key_long_mode_flag = 0;
	
	  g_pro.key_gtime_timer_define_flag = normal_time_mode;

	  fan_run_one_minute = 1;
	  g_pro.gTimer_fan_run_one_minute =0;

	  if(g_wifi.gwifi_link_net_state_flag == 1){
            MqttData_Publish_SetOpen(0);  
			osDelay(50);
	        MqttData_Publish_PowerOff_Ref() ;//
	        osDelay(100);
           
	  }
	  if(g_disp.g_second_disp_flag ==1){

	  
	   	  SendData_Set_Command(CMD_POWER,close);
          osDelay(5);
	
         

       }
	   g_pro.g_fan_switch_gears_flag++;
      gl_run.process_off_step = 1;
	   g_pro.key_set_temperature_flag=0;
	   g_pro.works_two_hours_interval_flag=0; //WT.EDIT 2025.05.07
	  

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

	 if(g_wifi.gwifi_link_net_state_flag == 1 && wifi_first_connect==0){
	 	    wifi_first_connect++;
            MqttData_Publish_SetOpen(0);  
			osDelay(50);
	        MqttData_Publish_PowerOff_Ref() ;//
	        osDelay(100);
           
	 }

   

   break;

   
   	}

}



