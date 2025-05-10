/*
 * bsp_mouse.c
 *
 *  Created on: 2025年3月7日
 *      Author: Administrator
 */
#include "bsp.h"

void mouse_open(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic On
}
void mouse_close(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off
}



