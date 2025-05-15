/*
 * interrupt_manager.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

/********************************************************************************
	**
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   static  uint16_t tm0;
  

    if(htim->Instance==TIM17){ //timer number14 is 100ms.
       tm0++;
	   if(tm0> 999){ //1s
	      tm0=0;
		   g_pro.gTimer_disp_time_second ++;
		   g_pro.gTimer_timer_time_second ++ ;
		   g_pro.gTimer_switch_temp_hum ++;
		   g_pro.gTimer_wifi_led_fast_blink ++ ;
		   g_pro.gTimer_switch_set_timer_times++;
		   g_pro.gTimer_input_set_temp_times++;
		   g_pro.gTimer_fan_run_one_minute++;
		   g_pro.gTimer_input_set_temp_timer++;
		   g_pro.gTimer_two_hours_counter++;
		   g_pro.gTimer_disp_temp_humidity_vlaue++;
		   g_pro.gTimer_display_adc_value++;
		   g_pro.gTimer_wifi_slowly_blink ++;
		    g_pro.gTimer_send_dht11_disp++;
			g_pro.gTimer_set_temp_counter++;
		   
		  

		   //wifi timer 
		   
	       g_wifi.gTimer_wifi_led_fast_blink ++ ;
		   g_wifi.gTimer_get_data_from_tencent_data++; 
		   g_wifi.gTimer_auto_link_net_time++;
		   g_wifi.gTimer_auto_detected_net_state_times++;
		   g_wifi.gTimer_link_net_timer_time++;
		   g_wifi.gTimer_get_beijing_time++;
		   g_wifi.gTimer_update_dht11_data++;
		

   

	   }
	   

   }
}

