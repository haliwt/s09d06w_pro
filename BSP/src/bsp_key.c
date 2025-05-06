/*
 * bsp_key.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"


#define MAX_TEMPERATURE 	40
#define MIN_TEMPERATURE 	20
#define MAX_TIMER_HOURS 	24
#define MIN_TIMER_HOURS 	0
#define TIMER_SECONDS_PER_MINUTE 60

#define CHECK_TIME_THRESHOLD_4S  200  // 4秒
#define CHECK_TIME_THRESHOLD_3S  150  // 3秒
#define TEMPERATURE_HIGH_THRESHOLD  39  // 高温阈值
#define TEMPERATURE_LOW_THRESHOLD   38  // 低温阈值
#define TEMPERATURE_DIFF_THRESHOLD  1   // 温度差阈值





KEY_PROCESS_TYPEDEF  g_key;

uint8_t gl_set_temperture_value;
uint8_t temperature_init_value ;
uint8_t  key_set_temperature_flag;
uint16_t check_time;
int8_t  gl_timer_minutes_value;
uint8_t define_timer_mode;
uint8_t key_up_down_pressed_flag;
uint8_t set_first_close_dry_flag;

uint8_t default_first_close_dry,key_set_timer_flag;


static void adjust_temperature(int8_t delta) ;
static void adjust_timer(int8_t delta) ;
static void handleTemperatureControl(void) ;
static void handleDefaultTemperatureControl(void);
static void setDryState(uint8_t state);
static void publishMqttData(DryState state, uint8_t temperature);

static void set_timer_mode(void);
static void set_normal_mode(void);


uint8_t readTemperature(void);


uint8_t current_temperature ; //WT.EDIT 2025.05.05



void key_referen_init(void)
{
  g_pro.key_set_temperature_flag=0;
  gl_timer_minutes_value =0;
  define_timer_mode=0;
  key_up_down_pressed_flag=0;
  key_set_timer_flag=0;
  
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
   #if 0
	if (p_gpiox->IDR & pinx)
    {
        return 1;   /* pinx的状态为1 */
    }
    else
    {
        return 0;   /* pinx的状态为0 */
    }
   #endif 
	 return (p_gpiox->IDR & pinx) ? 1 : 0;
}


/**
 * @brief  void adjust_temperature(int8_t delta)
 * @param  NO: 
 * @retval None
 */
static void adjust_temperature(int8_t delta) 
{
    if (temperature_init_value == 0) {
        temperature_init_value++;
        gl_set_temperture_value = (delta > 0) ? 21 : 39;
    } else {
        gl_set_temperture_value += delta;
        if (gl_set_temperture_value > MAX_TEMPERATURE) gl_set_temperture_value = MAX_TEMPERATURE;
        if (gl_set_temperture_value < MIN_TEMPERATURE) gl_set_temperture_value = MIN_TEMPERATURE;
    }
    g_pro.g_manual_shutoff_dry_flag = 0;
    key_up_down_pressed_flag = 1;
    g_pro.key_set_temperature_flag = 1;
		g_pro.gset_temperture_value= gl_set_temperture_value; //WT.EDIT 2025.05.05
    TM1639_Display_Temperature(gl_set_temperture_value);
    g_pro.gTimer_input_set_temp_times = 0;
    g_pro.gTimer_switch_temp_hum = 0;
}

/**
 * @brief  void adjust_timer(int8_t delta)
 * @param  NO: 
 * @retval None
 */
static void adjust_timer(int8_t delta) 
{
    g_pro.gTimer_switch_set_timer_times = 0;
    key_set_timer_flag = 1;
    g_pro.gdisp_timer_hours_value += delta;
    if (g_pro.gdisp_timer_hours_value > MAX_TIMER_HOURS) g_pro.gdisp_timer_hours_value = MAX_TIMER_HOURS;
    if (g_pro.gdisp_timer_hours_value < MIN_TIMER_HOURS) g_pro.gdisp_timer_hours_value = MIN_TIMER_HOURS;
    g_pro.g_disp_timer_or_temp_flag = input_set_timer_mode;//WT.EDIT 2025.04.23//input_temp_time_mode  ;
    TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value);
}



/**
 * @brief  key_dwon_fun(void)
 * @param  NO: 
 * @retval None
 */
void key_dwon_fun(void)
{
    switch (g_pro.key_gtime_timer_define_flag) {
        case normal_time_mode:
            adjust_temperature(-1);
            break;
        case input_set_timer_mode: //WT.EDIT 2025.04.23//timer_time_mode:
            adjust_timer(-1);
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
	 switch (g_pro.key_gtime_timer_define_flag) {
        case normal_time_mode:
            adjust_temperature(1);
            break;
        case input_set_timer_mode: //WT.EDIT 2025.04.23//:
            adjust_timer(1);
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
	
   
 
  if((g_pro.key_set_temperature_flag==1 || read_wifi_temperature_value()==1) && g_pro.gTimer_input_set_temp_timer >= 4)
	{
        g_pro.key_set_temperature_flag=2;
		if(read_wifi_temperature_value()==1){
			g_wifi.g_wifi_set_temp_flag=0;
			if(g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0){//the second displaybaord
		      SendWifiData_To_Data(0x11,gl_set_temperture_value);
			  osDelay(5);
			}
		}
		else{
			g_pro.gset_temperture_value = gl_set_temperture_value;
			if(g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0){//the second displaybaord
			SendWifiData_To_Data(0x11,gl_set_temperture_value);
			osDelay(5);
			}
		}
		set_first_close_dry_flag=0;
		g_pro.g_manual_shutoff_dry_flag =0;
		

        current_temperature = readTemperature();
        if (current_temperature > g_pro.gset_temperture_value){

			g_pro.gDry= DRY_STATE_OFF;
            setDryState(g_pro.gDry);
		
			if(g_wifi.gwifi_link_net_state_flag==wifi_link_success){
            publishMqttData(DRY_STATE_OFF, g_pro.gset_temperture_value);
			osDelay(100);
			}
            if (g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0) {
				
	                sendDisplayCommand(0x02,g_pro.gDry); // 关闭干燥功能
	                osDelay(5);
				
            }
        } 
		else if (current_temperature < g_pro.gset_temperture_value){
			if(g_pro.works_two_hours_interval_flag ==0){

			   g_pro.gDry = DRY_STATE_ON;
			   setDryState(g_pro.gDry);

		   }
			
			if(g_wifi.gwifi_link_net_state_flag==wifi_link_success){
            publishMqttData(DRY_STATE_ON, g_pro.gset_temperture_value);
			osDelay(100);
			}
            if (g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0) {
                sendDisplayCommand(0x02,0x01); // 打开干燥功能
                osDelay(5);
            }
        }
		key_up_down_pressed_flag=0;
		//g_disp.g_set_temp_value_flag =0;
    }
    else{

	
        if(g_pro.key_set_temperature_flag==2  && read_wifi_temperature_value()==0){
		    handleTemperatureControl();
           
          
        }
		else if(g_pro.key_set_temperature_flag==0){ //don't set temperature value 
				handleDefaultTemperatureControl();
		 
			}
        }
                
  }

// 读取温度值
uint8_t readTemperature(void) 
{
    return read_dht11_temperature_value();
}

/******************************************************************************
	*
	*Function Name:static void handleTemperatureControl(void) 
	*Funcion: // 处理温度控制逻辑
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
static void handleTemperatureControl(void) 
{
	//uint8_t current_temperature;
	static uint8_t check_time = 0;
    check_time++;
    if (check_time >= CHECK_TIME_THRESHOLD_4S) { // 4秒
        check_time = 0;
        current_temperature = readTemperature();

        if (current_temperature > g_pro.gset_temperture_value){
            g_pro.gDry = DRY_STATE_OFF;
		    DRY_CLOSE();//setDryState(g_pro.gDry);
			if(set_first_close_dry_flag ==0){

				set_first_close_dry_flag =1;
                 

			}
		    if(g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0){
				sendDisplayCommand(0x02,g_pro.gDry);
				osDelay(5);
		    }
			
		   if (g_wifi.gwifi_link_net_state_flag == 1){
		    MqttData_Publish_SetPtc(0x0);
			osDelay(50);
		   	}
			
        }
		else if (set_first_close_dry_flag == 0 && current_temperature < g_pro.gset_temperture_value ){

		    if(g_pro.g_manual_shutoff_dry_flag ==0){
			    if(g_pro.works_two_hours_interval_flag==0){

				    g_pro.gDry = DRY_STATE_ON;
					LED_DRY_ON();
					if(g_pro.works_two_hours_interval_flag == 0){
					     //setDryState(g_pro.gDry);
                         DRY_OPEN();
					 }
					

			    }
				
				if (g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0){
					sendDisplayCommand(0x02,0x01); // 打开干燥功能
					osDelay(5);
				}
				if (g_wifi.gwifi_link_net_state_flag == 1) {
					MqttData_Publish_SetPtc(0x01);
					osDelay(50);
				 }
           
            }

        }
        else if (current_temperature < (g_pro.gset_temperture_value - TEMPERATURE_DIFF_THRESHOLD)) {
            if(g_pro.g_manual_shutoff_dry_flag ==0){
				if(g_pro.works_two_hours_interval_flag == 0){
					 g_pro.gDry = DRY_STATE_ON;
					 LED_DRY_ON();
					 if(g_pro.works_two_hours_interval_flag == 0){
					     //setDryState(g_pro.gDry);
                         DRY_OPEN();
					 }

				}
				
				if (g_disp.g_second_disp_flag == 1 && g_disp.g_set_temp_value_flag ==0){
				   sendDisplayCommand(0x02,0x01); // 打开干燥功能
				   osDelay(5);
				}
				 if (g_wifi.gwifi_link_net_state_flag == 1) {
					MqttData_Publish_SetPtc(0x01);
					 osDelay(50);
				 }
           
            }
        }
    }
	g_disp.g_set_temp_value_flag =0;
}

/******************************************************************************
	*
	*Function Name:static void handleDefaultTemperatureControl(void)
	*Funcion: 处理默认温度控制逻辑
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
static void handleDefaultTemperatureControl(void) 
{
    static uint8_t check_time = 0;
	//uint8_t current_temperature;
    //check_time++;
    if ( g_pro.gTimer_disp_temp_humidity_vlaue > 2) { // 3秒
         g_pro.gTimer_disp_temp_humidity_vlaue = 0;
        current_temperature = readTemperature();

        if (current_temperature > 0x27) {
			if(default_first_close_dry==0)default_first_close_dry=1;
			g_pro.gDry =  DRY_STATE_OFF;
            setDryState(g_pro.gDry);
		    if(g_disp.g_second_disp_flag ==1){
		     sendDisplayCommand(0x02,0x0); // send data to the second displayboard .关闭干燥功能
			 osDelay(5);
			}
			 if (g_wifi.gwifi_link_net_state_flag == 1) {
			   	MqttData_Publish_SetPtc(DRY_STATE_OFF) ;
			    osDelay(50);
			 }
        } 
		else if(current_temperature <=0x26) {

		      if(default_first_close_dry==0){

			  if(g_pro.g_manual_shutoff_dry_flag ==0){

					g_pro.gDry= DRY_STATE_ON;

			        LED_DRY_ON();
					if(g_pro.works_two_hours_interval_flag ==0){

					  g_pro.gDry= DRY_STATE_ON;
					   setDryState(g_pro.gDry);


					if(g_disp.g_second_disp_flag ==1){
					sendDisplayCommand(0x02,0x01); // 打开干燥功能
					osDelay(5);
					}
					if (g_wifi.gwifi_link_net_state_flag == 1) {
					MqttData_Publish_SetPtc(DRY_STATE_ON);//publishMqttData(DRY_STATE_ON, current_temperature) ;
					osDelay(50);
					}

					}


			  	}
			 }
		     else if (current_temperature <= 0x26) {
	           if(g_pro.g_manual_shutoff_dry_flag ==0){

				 g_pro.gDry= DRY_STATE_ON;

			     LED_DRY_ON();
				if(g_pro.works_two_hours_interval_flag ==0){
					
					DRY_OPEN();//setDryState(g_pro.gDry);
					
				 if(g_disp.g_second_disp_flag ==1){
				  sendDisplayCommand(0x02,0x01); // 第二个显示板，打开干燥功能
				  osDelay(5);
				 }
				  if (g_wifi.gwifi_link_net_state_flag == 1) {
				  MqttData_Publish_SetPtc(DRY_STATE_ON);//publishMqttData(DRY_STATE_ON, current_temperature) ;
	              osDelay(50);
				  }
	            }

	           }
        }
    }
	}
}
/******************************************************************************
	*
	*Function Name:void set_timer_timing_value_handler(void)
	*Funcion: // 设置干燥状态
	*Input Ref: state: 0-off,1-on
	*Return Ref:NO
	*
******************************************************************************/
static void setDryState(uint8_t state) 
{
    //g_pro.gDry = state;
	
    if (state == DRY_STATE_ON){
        DRY_OPEN();
        LED_DRY_ON();
    } else {
        DRY_CLOSE();
        LED_DRY_OFF();
    }
}

// 发布MQTT数据
void publishMqttData(DryState state, uint8_t temperature) 
{
    if (g_wifi.gwifi_link_net_state_flag == 1) {
        MqttData_Publis_SetTemp(temperature);
        osDelay(50);
        MqttData_Publish_SetPtc(state);
        osDelay(50);
    }
}


/******************************************************************************
	*
	*Function Name:void set_timer_timing_value_handler(void)
	*Funcion: // 发送显示命令
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void sendDisplayCommand(uint8_t command,uint8_t data) 
{
    if(g_disp.g_second_disp_flag ==1){
	SendData_Set_Command(command, data);
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

   
   if(g_pro.key_gtime_timer_define_flag == input_set_timer_mode && g_pro.gTimer_switch_set_timer_times > 3 ){
   	      g_pro.gTimer_switch_set_timer_times=0;

          if(key_set_timer_flag==1){

			if(g_pro.gdisp_timer_hours_value>0){
			g_pro.g_disp_timer_or_temp_flag = timer_time_mode;
			g_pro.key_gtime_timer_define_flag = normal_time_mode; //define UP and down key is set temperature value 
			key_set_timer_flag++;
			g_pro.gTimer_timer_time_second=0;
			gl_timer_minutes_value=0;
			g_pro.gAI = 0;
			LED_AI_OFF();



			}
			else{
				g_pro.gAI = 1;
				LED_AI_ON();

				key_set_timer_flag=0;

				g_pro.g_disp_timer_or_temp_flag = normal_time_mode;
				g_pro.key_gtime_timer_define_flag = normal_time_mode;
			}
		}
		else{ //times is done ,exit this process

		     g_pro.key_gtime_timer_define_flag = normal_time_mode; //WT.EDIT 2025.04.30


		}
   	}
    else if(key_set_timer_flag==2){

       if(g_pro.gTimer_timer_time_second > 59){
	       g_pro.gTimer_timer_time_second=0;
		   #if TEST_UNIT
		   	 gl_timer_minutes_value = gl_timer_minutes_value - 40;
		   #else
		     gl_timer_minutes_value--;
		   #endif 

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
       return key_up_down_pressed_flag;
}


void mode_key_fun(void)
{

     g_key.mode_key_switch_time_mode = timer_time_mode;
     set_timer_mode();
		
  
  
}

// 提取的辅助函数
static void set_normal_mode(void)
{
    uint8_t error_dht11_flag;
	g_pro.gAI = 1;
    LED_AI_ON();
    HUMIDITY_ICON_OFF();
    TEMP_ICON_ON();
	
    error_dht11_flag=DHT11_Display_Data(0); // 显示温度
    if(error_dht11_flag == 0)DHT11_Display_Data(0);
    
}

static void set_timer_mode(void)
{
    g_pro.gAI = 0;
    LED_AI_OFF();
    HUMIDITY_ICON_OFF();
    TEMP_ICON_OFF();
   
    TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value);
}

