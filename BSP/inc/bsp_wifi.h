/*
 * bsp_wifi.h
 *
 *  Created on: 2025年3月7日
 *      Author: Administrator
 */

#ifndef __BSP_WIFI_H_
#define __BSP_WIFI_H_

#include "main.h"

extern uint8_t wifi_rx_inputBuf[1];


typedef enum{

  speed_fan_value = 0x1E,
  wifi_connect_net_flag = 0x1F,


}wifi_send_cmd_name;







typedef struct _wifi_state_ref{

  uint8_t gwifi_link_net_state_flag;

  uint8_t wifi_led_fast_blink_flag;

   uint8_t soft_ap_config_success;

   uint8_t link_net_step ;

   uint8_t g_wifi_set_temp_flag;


  
  uint8_t set_wind_speed_value;
  
  uint8_t wifi_get_beijing_step;

  uint8_t wifi_config_net_lable;

  uint8_t phone_set_temperature_value;
  
  uint8_t get_rx_beijing_time_enable;

  uint8_t wifi_rx_data_array[150];

  uint8_t wifi_rx_data_counter;
  uint8_t once_rx_data_done;

  uint8_t get_beijing_time_success ;

  uint8_t auto_link_cloud_flag;

  uint8_t soft_ap_config_flag;


   uint8_t linking_tencent_cloud_doing;

   uint8_t wifi_set_temperature_value;

   uint8_t app_timer_power_on_flag;




 // uint32_t randomName[1];

  uint8_t gTimer_get_beijing_time;
 

  uint8_t  gTimer_wifi_led_fast_blink;
  uint8_t  gTimer_get_data_from_tencent_data;
  uint8_t gTimer_auto_link_net_time;
  uint8_t gTimer_auto_detected_net_state_times;
  uint8_t gTimer_link_net_timer_time;
  uint8_t  gTimer_update_dht11_data;





}wifi_state_ref;

extern wifi_state_ref g_wifi;



void link_wifi_to_tencent_handler(uint8_t data);







#endif /* BSP_INC_BSP_WIFI_H_ */
