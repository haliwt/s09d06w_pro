/*
 * bsp.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_H_
#define BSP_INC_BSP_H_
#include "main.h"
#include "stdio.h"
#include "string.h"




#include "bsp_freertos.h"
#include "bsp_streamlight.h"
#include "bsp_key.h"
#include "bsp_fan.h"
#include "bsp_buzzer.h"
#include "bsp_tm1639.h"
#include "bsp_smg.h"
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_dht11.h"
#include "bsp_dry.h"
#include "bsp_mouse.h"
#include "bsp_plasma.h"
#include "bsp_disp.h"
#include "bsp_message.h"
#include "bsp_cmd_link.h"
#include "bsp_led_tape.h"
#include "interrupt_manager.h"

//wifi function
#include "bsp_wifi.h"
#include "bsp_esp8266.h"
#include "bsp_publish.h"
#include "bsp_subscribe.h"
#include "bsp_mqtt.h"
#include "bsp_wifi_decoder.h"
#include "bsp_connect_wifi.h"


//second display board
#include "bsp_second_display.h"


//mainboard

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

//freeRtos
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


#define  USE_FreeRTOS      1
  
#define  TEST_UNIT        1


#if USE_FreeRTOS == 1
	//#include "FreeRTOS.h"
	///#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef enum{
   power_off,
   power_on,
}process_state;

typedef enum{
	wifi_no_link,
	wifi_link_success,
}wifi_state;

typedef enum{
	normal_time_mode,
	timer_time_mode,
	beijing_time_mode,
	disp_timer_mode,
	disp_temp_hum_mode,
	input_set_timer_mode, //WT.EDIT 2025.04.23
	input_set_temperature_mode, //WT.EDIT 2025.04.23
	input_set_null
	
}time_state;

typedef struct _process{

   //main board reference

   uint16_t gTimer_two_hours_counter;
   uint8_t gpower_on;
   uint8_t gDry;
   uint8_t gPlasma;
   uint8_t gAI;
   uint8_t gMouse;
   uint8_t works_two_hours_interval_flag;
   uint8_t g_manual_shutoff_dry_flag;

   uint8_t g_humidity_value;
   uint8_t g_temperature_value;
  

   uint8_t g_fan_switch_gears_flag;
   uint8_t key_set_temperature_flag;
   

   uint8_t gTemp_value;
   uint8_t gset_temperture_value;

  
  
   
   uint8_t key_gtime_timer_define_flag;
   uint8_t g_disp_timer_or_temp_flag;
   uint8_t gdisp_hours_value;
   uint8_t gdisp_minutes_value;

   int8_t gdisp_timer_hours_value;
   
   
   uint8_t ptc_warning;
   uint8_t fan_warning ;

 //  uint8_t  gdisp_time_minutes;
 //  uint8_t  gdisp_timer_time_minutes;





   //timer
   
   uint8_t  gTimer_disp_time_second;
   uint8_t  gTimer_timer_time_second;
   uint8_t  gTimer_wifi_led_fast_blink;
   uint8_t  gTimer_switch_temp_hum;
   uint8_t  gTimer_switch_set_timer_times;
   uint8_t  gTimer_input_set_temp_times;
   uint8_t   gTimer_fan_run_one_minute;
   uint8_t   gTimer_input_set_temp_timer;
}process_t;

extern process_t g_pro;



void bsp_init(void);

void mainboard_fun_handler(void);

void mainboard_close_all_fun(void);

void works_run_two_hours_state(void);


#endif /* BSP_INC_BSP_H_ */
