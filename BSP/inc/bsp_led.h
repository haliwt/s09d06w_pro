/*
 * bsp_led.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "main.h"




#define LED_POWER_ON()           do{LED_POWER_GPIO_Port->BSRR |=(uint32_t)LED_POWER_Pin <<16;}while(0)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin, GPIO_PIN_RESET)
#define LED_POWER_OFF()          do{LED_POWER_GPIO_Port->BSRR |=LED_POWER_Pin;}while(0)//sys_write_gpio_pin_value(LED_POWER_GPIO_Port,LED_POWER_Pin,GPIO_PIN_SET)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin, GPIO_PIN_SET)

#define LED_AI_ON()              do{LED_AI_GPIO_Port->BSRR |=(uint32_t)LED_AI_Pin<<16;}while(0)
#define LED_AI_OFF()             do{LED_AI_GPIO_Port->BSRR |=LED_AI_Pin;}while(0)

#define LED_WIFI_ON()            do{LED_WIFI_GPIO_Port->BSRR |=(uint32_t)LED_WIFI_Pin<< 16;}while(0)
#define LED_WIFI_OFF()           do{LED_WIFI_GPIO_Port->BSRR |=LED_WIFI_Pin;}while(0)

/* LED取反定义 */
#define LED_WIFI_TOGGLE()        do{LED_WIFI_GPIO_Port->ODR ^= LED_WIFI_Pin;}while(0)      /* LED0 = !LED0 */





#define LED_TEMP_SINGLE_ON()     do{LED_TEMP_SYMBOL_GPIO_Port->BSRR |=(uint32_t)LED_TEMP_SYMBOL_Pin;}while(0)//        sys_write_gpio_pin_value(LED_TEMP_SYMBOL_GPIO_Port, LED_TEMP_SYMBOL_Pin, GPIO_PIN_RESET)
#define LED_TEMP_SINGLE_OFF()    do{LED_TEMP_SYMBOL_GPIO_Port->BSRR |=LED_TEMP_SYMBOL_Pin;}while(0)      //sys_write_gpio_pin_value(LED_TEMP_SYMBOL_GPIO_Port, LED_TEMP_SYMBOL_Pin, GPIO_PIN_SET)

#define LED_HUM_SINGLE_ON()      do{LED_HUM_SYMBOL_GPIO_Port->BSRR |=(uint32_t)LED_HUM_SYMBOL_Pin;}while(0)//sys_write_gpio_pin_value(LED_HUM_SYMBOL_GPIO_Port, LED_HUM_SYMBOL_Pin, GPIO_PIN_RESET)
#define LED_HUM_SINGLE_OFF()     do{LED_HUM_SYMBOL_GPIO_Port->BSRR |=LED_HUM_SYMBOL_Pin;}while(0)//sys_write_gpio_pin_value(LED_HUM_SYMBOL_GPIO_Port, LED_HUM_SYMBOL_Pin, GPIO_PIN_SET)



#define LED_PLASMA_ON()          sys_write_gpio_pin_value(LED_PLASMA_GPIO_Port, LED_PLASMA_Pin, GPIO_PIN_RESET)
#define LED_PLASMA_OFF()         sys_write_gpio_pin_value(LED_PLASMA_GPIO_Port, LED_PLASMA_Pin, GPIO_PIN_SET)

#define LED_MOUSE_ON()           sys_write_gpio_pin_value(LED_MOUSE_GPIO_Port, LED_MOUSE_Pin, GPIO_PIN_RESET)
#define LED_MOUSE_OFF()          sys_write_gpio_pin_value(LED_MOUSE_GPIO_Port, LED_MOUSE_Pin, GPIO_PIN_SET)

void power_on_led(void);
void power_off_led(void);
void LED_Power_Breathing(void);   // LED呼吸灯函数

void LED_FUN_ON(void);

void wifi_led_fast_blink(void);

void wifi_led_slowly_blink(void);

void power_on_smart_app_led(void);


#endif /* BSP_INC_BSP_LED_H_ */
