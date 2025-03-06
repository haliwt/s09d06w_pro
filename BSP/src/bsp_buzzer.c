/*
 * bsp_buzzer.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

void buzzer_sound(void)
{
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    osDelay(20);
    HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);


}





