/*
 * bsp_dht11.c
 *
 *  Created on: 2025年3月5日
 *      Author: Administrator
 */
#include "bsp.h"

static GPIO_InitTypeDef GPIO_InitStruct;

/**************************************************************************
 * static void DHT11_Mode_Out_PP(void)
 * 功能：配置DHT11引脚为推挽输出模式
 * 参数：无
 * 返回值：无
 *************************************************************************/
static void DHT11_Mode_Out_PP(void)
{
    GPIO_InitStruct.Pin = TEMP_SENSOR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TEMP_SENSOR_GPIO_Port, &GPIO_InitStruct);
}

/**************************************************************************
 * static void DHT11_Mode_IPU(void)
 * 功能：配置DHT11引脚为上拉输入模式
 * 参数：无
 * 返回值：无
 *************************************************************************/
static void DHT11_Mode_IPU(void)
{
    GPIO_InitStruct.Pin = TEMP_SENSOR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TEMP_SENSOR_GPIO_Port, &GPIO_InitStruct);
}

/**************************************************************************
 * static uint8_t DHT11_Read_Byte(void)
 * 功能：读取一个字节的数据
 * 参数：无
 * 返回值：读取到的字节数据，超时返回0
 *************************************************************************/
static uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, temp = 0;
    uint32_t timeout = 0;
    
    for(i = 0; i < 8; i++)
    {
        // 等待变高电平
        timeout = DHT11_TIMEOUT;
        while((!DHT11_PIN_READ()) && timeout) 
        {
            timeout--;
            delay_us(1);
        }
        if(!timeout) return 0;
        
        delay_us(40);  // 延时40us
        
        if(DHT11_PIN_READ())  // 如果还是高电平
        {
            temp |= (uint8_t)(0x01 << (7-i));  // 对应位置1
            
            // 等待变低电平
            timeout = DHT11_TIMEOUT;
            while((DHT11_PIN_READ()) && timeout)
            {
                timeout--;
                delay_us(1);
            }
            if(!timeout) return 0;
        }
    }
    return temp;
}

/**************************************************************************
 * void DHT11_Init(void)
 * 功能：初始化DHT11
 * 参数：无
 * 返回值：无
 *************************************************************************/
void DHT11_Init(void)
{
    DHT11_Mode_Out_PP();
    DHT11_PIN_SET();
    HAL_Delay(1000);  // 上电等待1s稳定
}

/**************************************************************************
 * DHT11_Status DHT11_Read_Data(DHT11_Data_TypeDef* data)
 * 功能：读取DHT11的温湿度数据
 * 参数：data - DHT11数据结构体指针
 * 返回值：DHT11_Status 类型的操作结果
 *************************************************************************/
DHT11_Status DHT11_Read_Data(DHT11_Data_TypeDef* data)
{
    uint8_t buf[5] = {0};
    uint32_t timeout = 0;
    
    // 主机发送开始信号
    DHT11_Mode_Out_PP();
    DHT11_PIN_RESET();
    delay_ms(20);    // 至少拉低18ms
    DHT11_PIN_SET();
    delay_us(20); // 主机拉高20~40us
    
    // 切换为输入模式
    DHT11_Mode_IPU();
    
    // 等待DHT11响应，先是低电平
    timeout = DHT11_TIMEOUT;
    while((DHT11_PIN_READ()) && timeout)
    {
        timeout--;
        delay_us(1);
    }
    if(!timeout) return DHT11_NO_RESPONSE;
    
    // 等待DHT11拉高
    timeout = DHT11_TIMEOUT;
    while((!DHT11_PIN_READ()) && timeout)
    {
        timeout--;
        delay_us(1);
    }
    if(!timeout) return DHT11_TIMEOUT_ERROR;
    
    // 等待DHT11拉低
    timeout = DHT11_TIMEOUT;
    while((DHT11_PIN_READ()) && timeout)
    {
        timeout--;
        delay_us(1);
    }
    if(!timeout) return DHT11_TIMEOUT_ERROR;
    
    // 读取40位数据
    for(uint8_t i = 0; i < 5; i++)
    {
        buf[i] = DHT11_Read_Byte();
        if(buf[i] == 0) return DHT11_TIMEOUT_ERROR;
    }
    
    // 验证校验和
    if(buf[0] + buf[2] != buf[4])
    {
        return DHT11_CHECKSUM_ERROR;
    }
    
    // 保存数据
    data->humidity = buf[0];
    data->temperature = buf[2];
    data->is_negative = 0;
    
    // 处理负温度（如果支持）
    if(buf[2] & 0x80)
    {
        data->temperature = buf[2] & 0x7F;
        data->is_negative = 1;
    }
    
    return DHT11_OK;
}

/**
 * @brief  在TM1639上显示DHT11的温湿度数据
 * @param  mode: 0-显示温度，1-显示湿度
 * @retval DHT11_Status 类型的操作结果
 */
DHT11_Status DHT11_Display_Data(uint8_t mode)
{
    DHT11_Data_TypeDef dht11_data;
    DHT11_Status status;
    
    // 读取DHT11数据
    status = DHT11_Read_Data(&dht11_data);
    if(status != DHT11_OK)
    {
        // 读取失败，显示错误代码
        TM1639_Display_3_Digit(status);
        return status;
    }
    
    // 根据模式显示温度或湿度
    if(mode == 0)
    {
        // 显示温度
        if(dht11_data.is_negative)
            TM1639_Display_Temperature(-dht11_data.temperature);
        else{
        	LED_TEMP_SINGLE_ON();
        	LED_HUM_SINGLE_OFF();

            TM1639_Display_Temperature(dht11_data.temperature);
        }
    }
    else
    {
        // 显示湿度
    	LED_TEMP_SINGLE_OFF();
    	LED_HUM_SINGLE_ON();
        TM1639_Display_Humidity(dht11_data.humidity);
    }
    
    return DHT11_OK;
}
