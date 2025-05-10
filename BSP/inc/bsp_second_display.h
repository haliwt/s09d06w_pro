/*
 * bsp_second_display.h
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */

#ifndef __BSP_SECOND_DISPLAY_H_
#define __BSP_SECOND_DISPLAY_H_
#include "main.h"


typedef enum{

   null_cmd,
   power_cmd = 0x01,
   dry_cmd =0x02,
   plasma_cmd  = 0x03,
   mouse_cmd = 0x04,
   connect_wifi_cmd = 0x05,
   buzzer_sound_cmd = 0x06,
   ai_mode_cmd = 0x07,
   ptc_warning_cmd = 0x08,
   fan_warning_cmd = 0x09,

   //
   second_disp_sync_cmd = 0x0A,
   
   


}second_disp_en;



typedef struct{

   uint8_t g_second_disp_flag ;
   uint8_t g_set_temp_value_flag;


}secnod_disp_typedef;

extern secnod_disp_typedef g_disp;


void ack_cmd_second_disp_hanlder(void);



#endif /* BSP_INC_BSP_SECOND_DISPLAY_H_ */
