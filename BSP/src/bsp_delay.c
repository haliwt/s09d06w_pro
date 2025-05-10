/*
 * bsp_delay.c
 *
 *  Created on: 2025年3月5日
 *
 *    Author: Administrator
 */
#include "bsp.h"

static uint32_t g_fac_us = 0;       /* us延时倍乘数 */


/**
 * @brief     初始化延迟函数
 * @param     sysclk: 系统时钟频率, 即CPU频率(HCLK), 72Mhz
 * @retval    无
 */
void delay_init(uint16_t sysclk)
{
#if SYS_SUPPORT_OS                          /* 如果需要支持OS. */
    uint32_t reload;
#endif
    SysTick->CTRL |= (1 << 2);              /* SYSTICK使用内部时钟源,频率为HCLK*/
    g_fac_us = sysclk;                      /* 不论是否使用OS,g_fac_us都需要使用 */
    SysTick->CTRL |= 1 << 0;                /* 使能Systick */
    SysTick->LOAD = 0X0FFFFFFF;             /* 注意systick计数器24位，所以这里设置最大重装载值 */
#if SYS_SUPPORT_OS                          /* 如果需要支持OS. */
    reload = sysclk;                        /* 每秒钟的计数次数 单位为M */
    reload *= 1000000 / delay_ostickspersec;/* 根据delay_ostickspersec设定溢出时间
                                             * reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右
                                             */
    g_fac_ms = 1000 / delay_ostickspersec;  /* 代表OS可以延时的最少单位 */
    SysTick->CTRL |= 1 << 1;                /* 开启SYSTICK中断 */
    SysTick->LOAD = reload;                 /* 每1/delay_ostickspersec秒中断一次 */
#endif
}


/**
 * @brief     延时nus
 * @note      无论是否使用OS, 都是用时钟摘取法来做us延时
 * @param     nus: 要延时的us数
 * @note      nus取值范围: 0 ~ (2^32 / fac_us) (fac_us一般等于系统主频, 自行套入计算)
 * @retval    无
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * g_fac_us;                 /* 需要的节拍数 */
    
#if SYS_SUPPORT_OS                          /* 如果需要支持OS */
    delay_osschedlock();                    /* 锁定 OS 的任务调度器 */
#endif

    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }

#if SYS_SUPPORT_OS                          /* 如果需要支持OS */
    delay_osschedunlock();                  /* 恢复 OS 的任务调度器 */
#endif 

}

/**
 * @brief     延时nms
 * @param     nms: 要延时的ms数 (0< nms <= (2^32 / fac_us / 1000))(fac_us一般等于系统主频, 自行套入计算)
 * @retval    无
 */
void delay_ms(uint16_t nms)
{
    
#if SYS_SUPPORT_OS  /* 如果需要支持OS, 则根据情况调用os延时以释放CPU */
    if (delay_osrunning && delay_osintnesting == 0)     /* 如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度) */
    {
        if (nms >= g_fac_ms)                            /* 延时的时间大于OS的最少时间周期 */
        {
            delay_ostimedly(nms / g_fac_ms);            /* OS延时 */
        }

        nms %= g_fac_ms;                                /* OS已经无法提供这么小的延时了,采用普通方式延时 */
    }
#endif

    delay_us((uint32_t)(nms * 1000));                   /* 普通方式延时 */
}


