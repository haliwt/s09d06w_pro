/*
 * bsp_dht11.h
 *
 *  Created on: 2025年3月5日
 *      Author: Administrator
 */

#ifndef __BSP_DHT11_H_
#define __BSP_DHT11_H_
#include "main.h"

// DHT11错误代码定义
typedef enum {
    DHT11_OK = 0,           // 操作成功
    DHT11_NO_RESPONSE,      // 传感器无响应
    DHT11_CHECKSUM_ERROR,   // 校验和错误
    DHT11_TIMEOUT_ERROR     // 通信超时错误
} DHT11_Status;

// DHT11 数据结构体
typedef struct {
    uint8_t humidity;       // 湿度值
    uint8_t temperature;    // 温度值
    int8_t  is_negative;    // 温度是否为负
} DHT11_Data_TypeDef;

// DHT11 显示模式定义
#define DHT11_DISPLAY_TEMP     0    // 显示温度
#define DHT11_DISPLAY_HUMI     1    // 显示湿度

#if 0
// DHT11 GPIO控制宏定义
#define DHT11_PIN_SET()     HAL_GPIO_WritePin(TEMP_SENSOR_GPIO_Port, TEMP_SENSOR_Pin, GPIO_PIN_SET)
#define DHT11_PIN_RESET()   HAL_GPIO_WritePin(TEMP_SENSOR_GPIO_Port, TEMP_SENSOR_Pin, GPIO_PIN_RESET)
#define DHT11_PIN_READ()    HAL_GPIO_ReadPin(TEMP_SENSOR_GPIO_Port, TEMP_SENSOR_Pin)

// 超时定义（单位：微秒）
#define DHT11_TIMEOUT       1000
#define DHT11_START_TIME    18000  // 18ms
#define DHT11_STOP_TIME     30     // 30us

// 函数声明
void DHT11_Init(void);
DHT11_Status DHT11_Read_Data(void);
DHT11_Status DHT11_Display_Data(uint8_t mode);   // 在TM1639上显示温湿度
#endif 

#define TEMP_ICON_ON()         	do{LED_TEMP_SYMBOL_GPIO_Port->BSRR |=(uint32_t)LED_TEMP_SYMBOL_Pin << 16 ;}while(0) //low level
#define TEMP_ICON_OFF()         do{LED_TEMP_SYMBOL_GPIO_Port->BSRR |=LED_TEMP_SYMBOL_Pin;}while(0) //high level

#define HUMIDITY_ICON_ON()      do{LED_HUM_SYMBOL_GPIO_Port->BSRR |=(uint32_t)LED_HUM_SYMBOL_Pin << 16;}while(0)
#define HUMIDITY_ICON_OFF()     do{LED_HUM_SYMBOL_GPIO_Port->BSRR |=LED_HUM_SYMBOL_Pin;}while(0)

/******************************************************************************************/
/* DHT11 引脚 定义 */

//TEMP_SENSOR_GPIO_Port



#define DHT11_DQ_GPIO_PORT                  TEMP_SENSOR_GPIO_Port
#define DHT11_DQ_GPIO_PIN                   TEMP_SENSOR_Pin
#define DHT11_DQ_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* GPIOA口时钟使能 */

/******************************************************************************************/

/* IO操作函数 */
#define DHT11_DQ_OUT(x)     do{ x ? \
                                HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)                                                /* 数据端口输出 */
#define DHT11_DQ_IN         HAL_GPIO_ReadPin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN)  /* 数据端口输入 */


uint8_t dht11_init(void);   /* 初始化DHT11 */
uint8_t dht11_check(void);  /* 检测是否存在DHT11 */
uint8_t dht11_read_data(uint8_t *temp,uint8_t *humi);   /* 读取温湿度 */

void Update_Dht11_Totencent_Value(void);

void Update_DHT11_ToDisplayBoard_Value(void);

DHT11_Status DHT11_Display_Data(uint8_t mode);

uint8_t read_dht11_temperature_value(void);

uint8_t read_sensor_dht11_data(void);

#endif /* BSP_INC_BSP_DHT11_H_ */
