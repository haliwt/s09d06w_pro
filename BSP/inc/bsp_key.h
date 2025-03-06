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

	KEY_UP ,
	KEY_DOWN ,

}KEYState_TypeDef;


typedef enum{

   MODE_NO_AI,
   MODE_AI,

}AIState_TypeDef;


typedef enum{

   KEY_NULL,
   KEY_POWER_ID = 0x01,
   KEY_MODEL_ID = 0x02,
   KEY_DOWN_ID = 0x04,
   KEY_UP_ID = 0x08,

   KEY_LONG_POWER = 0x81,
   KEY_LONG_MODE = 0x82

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

	uint8_t key_long_power_flag;
	uint8_t key_long_mode_flag;
}KEY_PROCESS_TYPEDEF;

extern KEY_PROCESS_TYPEDEF  g_key;







#endif /* BSP_INC_BSP_KEY_H_ */
