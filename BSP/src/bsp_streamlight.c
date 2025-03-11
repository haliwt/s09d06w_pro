/*
 * bsp_streamlight.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"



typedef enum{

  STREAMLIGHT_MODE_LEFT,
  STREAMLIGHT_MODE_RIGHT,
  STREAMLIGHT_MODE_BOTH


}streamlingh_direct;

static uint8_t Current_Mode;


// LED状态数组，每个bit代表一个LED
static uint8_t LED_States[4] = {0};  // GRID4-GRID7，每个GRID控制8个LED,GRID8 only has SEG_A,SEG_B
//static StreamLight_Mode_t Current_Mode = STREAMLIGHT_MODE_LEFT;
static uint16_t Stream_Speed = 100;   // 默认速度100ms
static uint32_t Last_Update = 0;      // 上次更新时间
static uint8_t Current_Position = 0;   // 当前LED位置

static uint32_t power_on_time = 0;    // 记录开机时间
static uint8_t led_test_done = 0;     // LED测试完成标志
static uint8_t led_testing = 0;       // LED测试进行中标志



/**
 * @brief  初始化流水灯
 * @param  None
 * @retval None
 */
void StreamLight_Init(void)
{
    // 清空所有LED
    memset(LED_States, 0, sizeof(LED_States));
    // 初始化第一个LED
    LED_States[0] = 0x01;
    
    // 更新显示
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID4_H, TM1639_ADDR_GRID4_L, LED_States[0]);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L, LED_States[1]);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L, LED_States[2]);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L, LED_States[3]);
    //GRID8
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L, 0xFC);
}

/**
 * @brief  设置流水灯模式
 * @param  mode: 流水灯模式
 * @retval None
 */
//void StreamLight_Set_Mode(StreamLight_Mode_t mode)
//{
//    Current_Mode = mode;
//    Current_Position = 0;
//    memset(LED_States, 0, sizeof(LED_States));
//    LED_States[0] = 0x01;
//}

/**
 * @brief  设置流水灯速度
 * @param  speed_ms: 速度（毫秒）
 * @retval None
 */
void StreamLight_Set_Speed(uint16_t speed_ms)
{
    Stream_Speed = speed_ms;
}

/**
 * @brief  关闭所有40个LED
 * @param  None
 * @retval None
 */
void StreamLight_All_LED_Off(void)
{
    // 清空LED状态数组
    memset(LED_States, 0, sizeof(LED_States));
    
    // 关闭GRID4-GRID8的所有LED
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID4_H, TM1639_ADDR_GRID4_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L, 0x00);
    // GRID8 clear SEG_A, SEG_B 
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L, 0xFC);
    
    // 重置位置计数器
    Current_Position = 0;
}

/**
 * @brief  检查并执行LED硬件测试
 * @param  None
 * @retval None
 */
void Check_LED_Hardware_Test(void)
{
    // 只在开机后3秒内且未完成测试时检查
    if(!led_test_done && (HAL_GetTick() - power_on_time < 3000) && (g_pro.gpower_on == power_on))
    {
        // 检测是否同时按下UP和DOWN键
        if(KEY_DOWN_VALUE() == KEY_DOWN && KEY_UP_VALUE() == KEY_DOWN)
        {
            // 设置测试标志
            led_testing = 1;
            
            // 先测试数码管 - 显示全8测试
            TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 0xFF); // 第1位显示8
            TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, 0xFF); // 第2位显示8
            TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, 0xFF); // 第3位显示8
            
            // 等待1秒让用户观察数码管显示
            HAL_Delay(1000);
            
            // LED测试
            uint8_t test_result = LED_TEST_OK;
            uint8_t i;
            uint8_t current_grid;    // 当前GRID (4-7)//(4-8)
            uint8_t current_bit;     // 当前bit位 (0-7)
            
            // 逐个点亮LED
            for(i = 0; i < 34; i++)
            {
                // 先关闭所有LED
                StreamLight_All_LED_Off();
                
                // 计算当前LED的GRID和bit位置
                current_grid = i / 8;         // 0-4 对应 GRID4-GRID8
                current_bit = i % 8;         // 0-7 对应每个GRID的8个LED
                
                // 根据GRID选择对应的地址并点亮LED
                switch(current_grid)
                {
                    case 0: // GRID4
                        TM1639_Write_Digit_Full(TM1639_ADDR_GRID4_H, TM1639_ADDR_GRID4_L, (1 << current_bit));
                        break;
                    case 1: // GRID5
                        TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L, (1 << current_bit));
                        break;
                    case 2: // GRID6
                        TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L, (1 << current_bit));
                        break;
                    case 3: // GRID7
                        TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L, (1 << current_bit));
                        break;
                    case 4: // GRID8
                       // TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L, (1 << current_bit));
                        break;
                }
                
                // 延时500ms
                HAL_Delay(500);
            }
            
            // 测试完成，显示结果
            if(test_result == LED_TEST_OK)
            {
                // 测试成功，显示"888"
                TM1639_Display_3_Digit(88);
            }
            else
            {
                // 测试被中止，显示"Err"
                TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 0x79); // E
                TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, 0x50); // r
                TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, 0x50); // r
            }
            
            // 等待2秒显示结果
            HAL_Delay(2000);
            
            // 关闭所有LED
            StreamLight_All_LED_Off();
            
            // 清除测试标志
            led_testing = 0;
            // 标记测试完成
            led_test_done = 1;
        }
    }
}

/**
 * @brief  获取LED测试状态
 * @param  None
 * @retval uint8_t 1:测试中 0:未在测试
 */
uint8_t Is_LED_Testing(void)
{
    return led_testing;
}

/**
 * @brief  初始化LED测试功能
 * @param  None
 * @retval None
 */
void StreamLight_Test_Init(void)
{
    power_on_time = HAL_GetTick();
    led_test_done = 0;
    led_testing = 0;
}

/**
 * @brief  更新流水灯状态
 * @param  None
 * @retval None
 */
void StreamLight_Update(void)
{
    uint32_t current_time = HAL_GetTick();
    
    // 检查是否需要更新
    if(current_time - Last_Update < Stream_Speed)
        return;
    
    Last_Update = current_time;
    
    // 根据不同模式更新LED状态
    switch(Current_Mode)
    {
        case STREAMLIGHT_MODE_LEFT:
            // 从左到右移动
            if(Current_Position < 33)  // 34个LED
            {
                uint8_t grid = Current_Position / 8;
                uint8_t bit = Current_Position % 8;
                
                // 清除当前LED
                LED_States[grid] &= ~(1 << bit);
                
                // 设置下一个LED
                Current_Position++;
                grid = Current_Position / 8;
                bit = Current_Position % 8;
                LED_States[grid] |= (1 << bit);
            }
            else
            {
                // 重新开始
                Current_Position = 0;
                memset(LED_States, 0, sizeof(LED_States));
                LED_States[0] = 0x01;
            }
            break;
            
        case STREAMLIGHT_MODE_RIGHT:
            // 从右到左移动
            if(Current_Position < 34)
            {
                uint8_t grid = (34 - Current_Position) / 8;
                uint8_t bit = (34 - Current_Position) % 8;
                
                // 清除当前LED
                LED_States[grid] &= ~(1 << bit);
                
                // 设置下一个LED
                Current_Position++;
                grid = (34 - Current_Position) / 8;
                bit = (34 - Current_Position) % 8;
                LED_States[grid] |= (1 << bit);
            }
            else
            {
                // 重新开始
                Current_Position = 0;
                memset(LED_States, 0, sizeof(LED_States));
                LED_States[3] = 0x80;  // 最右边的LED
            }
            break;
            
        case STREAMLIGHT_MODE_BOTH:
            // 从中间向两边移动
            if(Current_Position < 20)  // 移动到两边
            {
                uint8_t left_grid = (19 - Current_Position) / 8;
                uint8_t left_bit = (19 - Current_Position) % 8;
                uint8_t right_grid = (20 + Current_Position) / 8;
                uint8_t right_bit = (20 + Current_Position) % 8;
                
                // 清除当前LED
                if(Current_Position > 0)
                {
                    LED_States[left_grid] &= ~(1 << left_bit);
                    LED_States[right_grid] &= ~(1 << right_bit);
                }
                
                // 设置下一个LED
                Current_Position++;
                left_grid = (19 - Current_Position) / 8;
                left_bit = (19 - Current_Position) % 8;
                right_grid = (20 + Current_Position) / 8;
                right_bit = (20 + Current_Position) % 8;
                LED_States[left_grid] |= (1 << left_bit);
                LED_States[right_grid] |= (1 << right_bit);
            }
            else
            {
                // 重新开始
                Current_Position = 0;
                memset(LED_States, 0, sizeof(LED_States));
                LED_States[2] = 0x0C;  // 中间两个LED
            }
            break;
    }
    
    // 更新显示
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID4_H, TM1639_ADDR_GRID4_L, LED_States[0]);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L, LED_States[1]);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L, LED_States[2]);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L, LED_States[3]);
    //GRID8
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L, SEG_A);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L, SEG_B);
}
