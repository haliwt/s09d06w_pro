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

	   }
	   

   }
}

