/*
 * bsp_key.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_KEY_H_
#define BSP_INC_BSP_KEY_H_
#include "main.h"


// GPIOD 寄存器基地址
//#define GPIOD_BASE        0x48000C00UL

// GPIOD 输入数据寄存器 (IDR) 偏移量
#define GPIOD_IDR_OFFSET  0x10

// GPIOD_IDR 寄存器指针
//#define GPIOD_IDR         (*(volatile uint32_t *)(GPIOD_BASE + GPIOD_IDR_OFFSET))

// 引脚掩码
#define GPIO_1        (1U << 1)  // GPIO_1
#define GPIO_2        (1U << 2)  // GPIO_2
#define GPIO_3        (1U << 3)  // GPIO_3
#define GPIO_4        (1U << 4)  // GPIO_4




typedef enum
{
   KEY_DOWN ,

	KEY_UP 
}KEYState_TypeDef;


typedef enum{

   MODE_NO_AI,
   MODE_AI,

}AIState_TypeDef;


// 干燥状态枚举
typedef enum {
    DRY_STATE_OFF = 0,
    DRY_STATE_ON = 1
} DryState;


typedef enum{

   KEY_NULL,
   KEY_POWER_ID = 0x01,
   KEY_MODEL_ID = 0x02,
   KEY_DOWN_ID = 0x04,
   KEY_UP_ID = 0x08,

   KEY_DWON_RUN_ID= 0x14,

   KEY_LONG_POWER = 0x81,
   KEY_LONG_MODE = 0x82,

   

}KEYName_Typedef;

#if 0
#define KEY_POWER_VALUE()           HAL_GPIO_ReadPin(GPIOD ,POWER_KEY_Pin)
#define KEY_MODE_VALUE()            HAL_GPIO_ReadPin(GPIOD,MODEL_KEY_Pin)
#define KEY_DOWN_VALUE()            HAL_GPIO_ReadPin(GPIOD,DEC_KEY_Pin)
#define KEY_UP_VALUE()              HAL_GPIO_ReadPin(GPIOD,ADD_KEY_Pin)

#else

#define KEY_POWER_VALUE()          sys_read_gpio_pin_value(GPIOD, KEY_POWER_Pin) //GPIO_1 按键按下，返回值: 1
#define KEY_MODE_VALUE()           sys_read_gpio_pin_value(GPIOD, KEY_MODE_Pin)
#define KEY_DOWN_VALUE()           sys_read_gpio_pin_value(GPIOD, KEY_DOWN_Pin)
#define KEY_UP_VALUE()             sys_read_gpio_pin_value(GPIOD, KEY_UP_Pin)

#endif


typedef struct __KEY_PROCESS{

	uint8_t key_power_flag;
	uint8_t key_mode_flag;
	uint8_t key_down_flag;
	uint8_t key_up_flag;
	uint8_t  mode_key_switch_time_mode;

	uint8_t key_long_power_flag;
	uint8_t key_long_mode_flag;
}KEY_PROCESS_TYPEDEF;

extern KEY_PROCESS_TYPEDEF  g_key;

void key_referen_init(void);


uint8_t sys_read_gpio_pin_value(GPIO_TypeDef *p_gpiox, uint16_t pinx);

void sys_write_gpio_pin_value(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status);

void key_dwon_fun(void);
void key_up_fun(void);

uint8_t read_key_up_down_mode(void);

void set_temperature_value_handler(void);

void set_timer_timing_value_handler(void);

void sendDisplayCommand(uint8_t command,uint8_t data) ;
void mode_key_fun(void);


#endif /* BSP_INC_BSP_KEY_H_ */
