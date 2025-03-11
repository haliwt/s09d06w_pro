/*
 * bsp_key.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

KEY_PROCESS_TYPEDEF  g_key;

uint8_t glset_temperture_value;
uint8_t temperature_init_value ;
uint8_t  key_set_temperature_flag;
uint16_t check_time;
int8_t  gl_timer_minutes_value;
uint8_t define_timer_mode,key_up_down_mode;
uint8_t input_key_timer_flag;

void key_referen_init(void)
{
  key_set_temperature_flag=0;
  gl_timer_minutes_value =0;
  define_timer_mode=0;
  key_up_down_mode =0 ;
  
}

/**
 * @brief       设置GPIO某个引脚的输出状态
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1, 引脚状态(仅最低位有效), 设置如下:
 *   @arg       0, 输出低电平
 *   @arg       1, 输出高电平
 * @retval      无
 */
void sys_write_gpio_pin_value(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status)
{
    if (status & 0X01)
    {
        p_gpiox->BSRR |= pinx;  /* 设置GPIOx的pinx为1 */
    }
    else
    {
        p_gpiox->BSRR |= (uint32_t)pinx << 16;  /* 设置GPIOx的pinx为0 */
    }
}

/**
 * @brief       读取GPIO某个引脚的状态
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @retval      返回引脚状态, 0, 低电平; 1, 高电平
 */
uint8_t sys_read_gpio_pin_value(GPIO_TypeDef *p_gpiox, uint16_t pinx)
{
    if (p_gpiox->IDR & pinx)
    {
        return 1;   /* pinx的状态为1 */
    }
    else
    {
        return 0;   /* pinx的状态为0 */
    }
}



/**
 * @brief  key_dwon_fun(void)
 * @param  NO: 
 * @retval None
 */
void key_dwon_fun(void)
{

  
  switch(g_pro.key_gtime_timer_define_flag ){
    case normal_time_mode: //set temperature number 
       
       if(temperature_init_value==0){
		   temperature_init_value++;

	       glset_temperture_value = 39;

	   }
	   else{
	       glset_temperture_value --; //20 ~40 degree
		   if(glset_temperture_value < 20)glset_temperture_value =20;

	   }
	   
        key_up_down_mode=1;
	   g_pro.gclose_ptc_flag =0;
       TM1639_Display_Temperature(glset_temperture_value);
	   g_pro.gTimer_input_set_temp_times=0;
	   key_set_temperature_flag =1;//
	
	   g_pro.gTimer_switch_temp_hum = 0;
       
    break;
    case timer_time_mode://set timer timing numbers 
          g_pro.gTimer_switch_set_timer_times =0;
		    input_key_timer_flag=1;
		 
          g_pro.gdisp_timer_hours_value--;
		  if(g_pro.gdisp_timer_hours_value < 0) g_pro.gdisp_timer_hours_value=0; //24 hours
		   key_up_down_mode=2;
		  TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value);
   
    break;
    default:
        break;
  }

}
/**
 * @brief : void key_up_fun(void)
 * @param  NO: 
 * @retval None
 */
 void key_up_fun(void)
{
	switch(g_pro.key_gtime_timer_define_flag ){
    case normal_time_mode: //set temperature number 
    
		if(temperature_init_value==0){
		   temperature_init_value++;

	       glset_temperture_value = 21;

	   }
       else{
		   glset_temperture_value ++; //20 ~40 degree
		   if(glset_temperture_value > 40)glset_temperture_value =40;
		  
       }
	    key_up_down_mode=1;
	    g_pro.gclose_ptc_flag =0;
       
       TM1639_Display_Temperature(glset_temperture_value);
	   g_pro.gTimer_input_set_temp_times=0;
	   key_set_temperature_flag=1;//
	   g_pro.gtimer_timing_mode_enable = input_temp_time_mode;
	   g_pro.gTimer_switch_temp_hum = 0;
	   
       
    break;
    case timer_time_mode://set timer timing numbers 
   
		g_pro.gTimer_switch_set_timer_times=0;
        input_key_timer_flag=1;

        g_pro.gdisp_timer_hours_value++;
		if(g_pro.gdisp_timer_hours_value > 24) g_pro.gdisp_timer_hours_value=24; //24 hours
		 key_up_down_mode=2;
		
		TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value);
    
   
    break;
    default:
        break;
  }

}

/******************************************************************************
	*
	*Function Name:void bsp_init(void)
	*Funcion: 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void set_temperature_value_handler(void)
{
	
    uint8_t real_read_temperture_value;
    static uint8_t first_close_dry_flag, donot_define_close;
    if(key_set_temperature_flag == 1 && g_pro.gTimer_input_set_temp_temp_time >= 3)
	{
		key_set_temperature_flag++;//
		 key_up_down_mode=0;
		
		g_pro.gset_temperture_value = glset_temperture_value;
		g_pro.gTimer_switch_temp_hum=5;
        real_read_temperture_value = read_dht11_temperature_value();
        first_close_dry_flag=0;
        if(real_read_temperture_value > g_pro.gset_temperture_value){
            DRY_CLOSE();
        }
        else{
            DRY_OPEN();
        }
    }
    else{
        if(key_set_temperature_flag==2){
             check_time++;
             if(check_time >= 200){ //4s 
                 check_time = 0;
                 real_read_temperture_value = read_dht11_temperature_value();
                 if(real_read_temperture_value > g_pro.gset_temperture_value){
                     if(first_close_dry_flag==0){
                         first_close_dry_flag=1;
                         DRY_CLOSE();
					     LED_DRY_OFF();
                     }
                     else{

                        DRY_CLOSE();  
						LED_DRY_OFF();
                     }
                     
                 }
                 else{
				 	 real_read_temperture_value = read_dht11_temperature_value();
				 	 if(first_close_dry_flag==1 && read_wifi_dry_value()==0){
					 	if(g_pro.gset_temperture_value > 21){ //温度在 20 ~ 40度
						    if(real_read_temperture_value <= (g_pro.gset_temperture_value -2)){
								 DRY_OPEN();
								 LED_DRY_ON();
						    }
					 	}
						else{

						DRY_OPEN();
						LED_DRY_ON();

						}
					 }
					 else if(read_wifi_dry_value()==0){
                      DRY_OPEN();
					  LED_DRY_ON();

					}
                 }
             }
          
        }
		else if(key_set_temperature_flag==0){

             check_time++;
             if(check_time > 150){ //50~=1s ,3s
                 check_time = 0;
                 real_read_temperture_value = read_dht11_temperature_value();
                 if(real_read_temperture_value > 39){ //39 degree
                     if(donot_define_close==0){
					     donot_define_close++;

					 }
                     DRY_CLOSE();  
					 LED_DRY_OFF();
                 }
                 else{

				     real_read_temperture_value = read_dht11_temperature_value();
				 	 if(donot_define_close==1 && read_wifi_dry_value()==0){
					 	
					    if(real_read_temperture_value <= 37){
								 DRY_OPEN();
								 LED_DRY_ON();
						  }
				 	  }
					  else if(read_wifi_dry_value()==0){
	                     DRY_OPEN();
						 LED_DRY_ON();
					  	}
					 }
                 }
        }
                
    }
        
 }


/******************************************************************************
	*
	*Function Name:void set_timer_timing_value_handler(void)
	*Funcion: 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void set_timer_timing_value_handler(void)
{

    static uint8_t define_timer_mode;
	if(g_pro.gtimer_timing_mode_enable == timer_time_mode &&  input_key_timer_flag==1){

	 if(g_pro.gTimer_switch_set_timer_times > 3){
		g_pro.gTimer_switch_set_timer_times=0;
	
		 key_up_down_mode=0;
	     input_key_timer_flag=0;

    	if(g_pro.gdisp_timer_hours_value>0){
          g_pro.gtimer_timing_mode_enable = timer_time_mode;
		  define_timer_mode = timer_time_mode;
		   g_pro.gTimer_timer_time_second=0;
		   gl_timer_minutes_value=0;
		   g_pro.gAI = 0;
		   LED_AI_OFF();
		   
    	}
    	else{
		    g_pro.gAI = 1;
			LED_AI_OFF();
		   key_up_down_mode=0;
		   input_key_timer_flag=0;
    	   g_pro.gtimer_timing_mode_enable = normal_time_mode;
		   define_timer_mode = normal_time_mode;

    	}
    }
    }
	if(g_pro.gtimer_timing_mode_enable == timer_time_mode &&  input_key_timer_flag==0){

       if(g_pro.gTimer_timer_time_second > 59){
	       g_pro.gTimer_timer_time_second=0;
		   gl_timer_minutes_value = gl_timer_minutes_value - 40;
		  // gl_timer_minutes_value--;

		   if(gl_timer_minutes_value< 0){
			  gl_timer_minutes_value =59;
			  g_pro.gdisp_timer_hours_value--;

			  if(g_pro.gdisp_timer_hours_value < 0){

			       g_pro.gpower_on = power_off;
			  	
			  }
			 

            }



	   }

   }
}


uint8_t read_key_up_down_mode(void)
{
       return key_up_down_mode;
}

