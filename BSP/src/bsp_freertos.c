#include "bsp.h"
#include "bsp_streamlight.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define POWER_BIT_0	        (1 << 0)
#define MODE_BIT_1	        (1 << 1)
#define DOWN_BIT_2           (1 << 2)
#define UP_BIT_3           (1 << 3)

#define KEY_LONG_POWER_BIT_4      (1<<4)
#define KEY_LONG_MODE_BIT_5			(1<<5)


#define MODE_LONG_KEY_8         (1 << 8)
#define DECODER_BIT_9          (1<< 9)


uint8_t inputBuf[1];


/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
static void vTaskRunPro(void *pvParameters);
static void vTaskDecoderPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);



/* 创建任务通信机制 */
//static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskDecoderPro= NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static QueueHandle_t xQueue1 = NULL;
//static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;



///#define QUEUE_LENGTH            1                   /* 队列支持的消息个数 */
///#define QUEUE_ITEM_SIZE         sizeof(uint32_t)    /* 队列中每条消息的大小 */
//#define SEMAPHORE_BINARY_LENGTH 1                   /* 二值信号量的有效长度 */
//#define QUEUESET_LENGTH         ((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */



typedef struct Msg
{

	uint8_t  usData[12];
    uint8_t  ucMessageID;
    uint8_t  rx_data_counter;
    uint8_t  disp_rx_cmd_done_flag;
    uint8_t  bcc_check_code;
    volatile uint8_t ulid;

}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */
uint8_t ucKeyCode;
uint8_t uckey_number;
uint8_t key_power_flag,decoder_flag ;
uint8_t check_code;

uint8_t power_on_key_counter, mode_key_counter;


void freeRTOS_Handler(void)
{
    /* 创建任务 */
	  AppTaskCreate();

	  /* 创建任务通信机制 */
	//  AppObjCreate();

	  /* 启动调度，开始执行任劄1�7 */
	   vTaskStartScheduler();
}

/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: vTaskTaskUserIF
*	功能说明: 接口消息处理〄1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 1  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskDecoderPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(5000); /* 设置最大等待时间为30ms */
	uint32_t ulValue;


    while(1)
    {

	xResult = xTaskNotifyWait(0x00000000,
								  0xFFFFFFFF,     /* Reset the notification value to 0 on */
								&ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
								xMaxBlockTime);//portMAX_DELAY);  /* 阻塞时间30ms，释放CUP控制权,给其它任务执行的权限*/

	if( xResult == pdPASS )
	{
			/* 接收到消息，检测那个位被按下 */

		if((ulValue & DECODER_BIT_9) != 0){
  
			    gl_tMsg.disp_rx_cmd_done_flag = 0;
				check_code =  bcc_check(gl_tMsg.usData,gl_tMsg.ulid);

				if(check_code == gl_tMsg.bcc_check_code ){

				 receive_data_from_displayboard(gl_tMsg.usData);
				// memset(gl_tMsg.usData, 0, 4); // 初始化为 0
				 
			}
				
		 }
	 }
   }
}

/**********************************************************************************************************
*	Function Name: static void vTaskRunPro(void *pvParameters)
*	Function:
*	Input Ref: pvParameters 是在创建该任务时传��的形参
*	Return Ref:
*   priority: 1  (数值越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskRunPro(void *pvParameters)
{
  while(1){
    
	if(g_key.key_power_flag == KEY_POWER_ID){

		power_on_key_counter ++ ;

		if(KEY_POWER_VALUE() ==KEY_UP && power_on_key_counter < 60 && g_key.key_long_power_flag != KEY_LONG_POWER){
			g_key.key_power_flag=0;
			power_on_key_counter=0;
			buzzer_sound();
			if(g_pro.gpower_on == power_off){
				g_pro.gpower_on = power_on;

			}
			else{

				g_pro.gpower_on = power_off;
			}
		}
		else if(KEY_POWER_VALUE() ==KEY_DOWN && g_pro.gpower_on == power_on && (power_on_key_counter  >= 60 && power_on_key_counter < 200)){
			g_key.key_power_flag=0;
			power_on_key_counter=202;
			g_key.key_long_power_flag =  KEY_LONG_POWER; //wifi led blink fast .
			g_wifi.gTimer_wifi_led_fast_blink = 0; //time start 120s ->look for wifi information 120s,timer.
			g_wifi.gwifi_link_net_state_flag=0 ; //clear wifi link net flag .repeat be detected wifi state.
			g_wifi.wifi_led_fast_blink_flag=1;   // led blink flag .
			g_wifi.link_net_step = 0; //WT.EDIT 2025.05.12
			g_pro.first_connect_wifi_flag=0;
			buzzer_sound();
			if(g_disp.g_second_disp_flag ==1){

				SendData_Set_Command(CMD_CONNECT_WIFI,0x01);
				osDelay(5);
			}
		}
	}
	else if(g_key.key_mode_flag == KEY_MODEL_ID ){ //&& MODEL_KEY_VALUE()==KEY_UP){

		mode_key_counter++ ;
		if(KEY_MODE_VALUE() == KEY_UP && mode_key_counter < 60){
			g_key.key_mode_flag = KEY_NULL;
			mode_key_counter=0;
			g_pro.gTimer_switch_set_timer_times = 0;
			if(g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
			buzzer_sound();
	        mode_key_fun();
		    }
		}
		else if(KEY_MODE_VALUE() == KEY_DOWN && mode_key_counter >= 60 && mode_key_counter < 200){

			g_key.key_mode_flag = KEY_NULL;
			mode_key_counter=202;
		    if(g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
				buzzer_sound();
				g_pro.key_gtime_timer_define_flag = input_set_timer_mode;
				g_pro.gTimer_switch_set_timer_times = 0;
		        HUMIDITY_ICON_OFF(); //WT.EDIT 2025.04.23
				TEMP_ICON_OFF();//WT.EDIT 2025.04.23
				TM1639_Display_3_Digit(g_pro.gdisp_timer_hours_value);//WT.EDIT 2025.04.23
		    }

		}
	}
	else if(g_key.key_down_flag ==KEY_DOWN_ID && KEY_DOWN_VALUE() == KEY_UP ){// && DEC_KEY_VALUE()==KEY_UP){
		g_key.key_down_flag = KEY_NULL;
		g_key.mode_key_switch_time_mode= normal_time_mode; //WT.EDIT 2025.04.30
		if(g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
		   buzzer_sound();

		    key_dwon_fun();
		}
	}
	else if(g_key.key_up_flag ==KEY_UP_ID && KEY_UP_VALUE() == KEY_UP ){ // && ADD_KEY_VALUE()==KEY_UP){
		g_key.key_up_flag =KEY_NULL;
		g_key.mode_key_switch_time_mode= normal_time_mode; //WT.EDIT 2025.04.30
		
		if(g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
		   buzzer_sound();
           key_up_fun();

		}
	}

	power_onoff_handler(g_pro.gpower_on);

	

	if(g_wifi.wifi_led_fast_blink_flag==0 ){
		wifi_communication_tnecent_handler();//
		getBeijingTime_cofirmLinkNetState_handler();
		wifi_auto_detected_link_state();
	}

	//copy_cmd_hanlder();
    //ack_cmd_second_disp_hanlder();
	vTaskDelay(10);

	  
    }
	  
}


/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function:
*	Input Ref: pvParameters 是在创建该任务时传递的形参
*	Return Ref:
*	priority: 3  (数值越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
	BaseType_t xResult;
    //const TickType_t xMaxBlockTime = pdMS_TO_TICKS(2000); /* 设置最大等待时间为30ms */
	uint32_t ulValue;
   

    while(1)
    {

      xResult = xTaskNotifyWait(0x00000000,
						           0xFFFFFFFF,
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
								  portMAX_DELAY);  /* portMAX_DELAY  最大允许延迟时间 */
        if( xResult == pdPASS ){

            /* 接收到消息，检测那个位被按下 */
            if((ulValue & POWER_BIT_0 ) != 0)
            {
           		  power_on_key_counter=0;
                  g_key.key_power_flag = KEY_POWER_ID;

            }
            else if((ulValue & MODE_BIT_1 ) != 0){   /* 接收到消息，检测那个位被按下 */
            	 if(g_pro.gpower_on == power_on){
				 	 mode_key_counter=0;
            	     g_key.key_mode_flag = KEY_MODEL_ID;

            	 }

             }
            else if((ulValue & DOWN_BIT_2 ) != 0){
            	  if(g_pro.gpower_on == power_on){
				  	 
            	       g_key.key_down_flag =KEY_DOWN_ID;
            	             
            	  }

            }
            else if((ulValue & UP_BIT_3 ) != 0){   /* 接收到消息，检测那个位被按下 */
            	 if(g_pro.gpower_on == power_on){
            	      g_key.key_up_flag = KEY_UP_ID;
            	                 
            	 }
            }
            
          }

        }
}
 /**********************************************************************************************************
*	Function Name: AppTaskCreate
*	功能说明: 创建应用任务
*	Input Ref:
*	Return Ref:
**********************************************************************************************************/
void AppTaskCreate (void)
{

  xTaskCreate( vTaskDecoderPro,    		/* 任务函数  */
                 "vTaskDecoderPro",  		/* 任务各1�7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskDecoderPro); /* 任务句柄  */

  xTaskCreate( vTaskRunPro,    		/* 任务函数  */
                 "vTaskRunPro",  		/* 任务各1�7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskRunPro); /* 任务句柄  */

	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}




/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state,rx_end_flag ;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;


    if(huart->Instance == USART1) // mainBoard receive data from display board send data USART1
	{

     //  DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0xA5){  // 0xA5 -- second display board ID
               gl_tMsg.rx_data_counter=0;
				         gl_tMsg.ulid=0;
               gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
				state=1; //=1

             }
            else
                state=0;
		break;


		case 1: //#1

            if(gl_tMsg.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;

	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];


              if(rx_end_flag == 1){

                state = 0;

                gl_tMsg.ulid = gl_tMsg.rx_data_counter;
                rx_end_flag=0;

                gl_tMsg.rx_data_counter =0;

                gl_tMsg.disp_rx_cmd_done_flag = 1 ;

                gl_tMsg.bcc_check_code=inputBuf[0];

                #if 1
                xTaskNotifyFromISR(xHandleTaskDecoderPro,  /* 目标任务 */
                                    DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                                    eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                                    &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				#endif 

              }

              }

              if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE && rx_end_flag == 0 &&   gl_tMsg.rx_data_counter > 4){

                     rx_end_flag = 1 ;

              }

        break;



		}

        //  ENABLE_INT();
    __HAL_UART_CLEAR_OREFLAG(&huart1);
	HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte

   }
   else if(huart->Instance==USART2) //WIFI USART2
	   {
	  //  DISABLE_INT();
		if(g_wifi.linking_tencent_cloud_doing ==1){
	
			   g_wifi.wifi_rx_data_array[g_wifi.wifi_rx_data_counter] =wifi_rx_inputBuf[0];
			   g_wifi.wifi_rx_data_counter++;
	
               if(g_wifi.wifi_rx_data_counter >1){
			   if(g_wifi.wifi_rx_data_array[g_wifi.wifi_rx_data_counter-2]==0x0D \
			   	&& g_wifi.wifi_rx_data_array[g_wifi.wifi_rx_data_counter-1]==0x0A){
			   
				   
				   Wifi_Rx_InputInfo_Handler();
				   g_wifi.wifi_rx_data_counter=0;
			   }
               }
	
		       
		}
		else{
	
			   if(g_wifi.get_rx_beijing_time_enable==1){
					  g_wifi.wifi_rx_data_array[g_wifi.wifi_rx_data_counter] = wifi_rx_inputBuf[0];
					  g_wifi.wifi_rx_data_counter++;
					   
			   }
			   else
				   Subscribe_Rx_Interrupt_Handler();
		}
		//	ENABLE_INT();
		 __HAL_UART_CLEAR_OREFLAG(&huart2);
		 HAL_UART_Receive_IT(&huart2,wifi_rx_inputBuf,1);
	}
}
/**********************************************************
**********************************************************/
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);

   switch(GPIO_Pin){

   case KEY_POWER_Pin:
       // DISABLE_INT(); //WT.EDIT 2024.08.15 modify.
        if(KEY_POWER_VALUE()  ==KEY_DOWN){

        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
        POWER_BIT_0,      /* 设置目标任务事件标志位bit0  */
        eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


        }

     //  ENABLE_INT();
   break;

   case KEY_MODE_Pin:

       if(KEY_MODE_VALUE() == KEY_DOWN && g_pro.gpower_on == power_on){
        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
               MODE_BIT_1,     /* 设置目标任务事件标志位bit0  */
               eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
               &xHigherPriorityTaskWoken);

        /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


       }

      

   break;


   case KEY_DOWN_Pin:
      // DISABLE_INT();
       if(KEY_DOWN_VALUE() == KEY_DOWN && g_pro.gpower_on == power_on){
        
         xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                DOWN_BIT_2,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        
        }
     ///  ENABLE_INT();
   break;

   case KEY_UP_Pin:
      ///   DISABLE_INT();
        if(KEY_UP_VALUE() == KEY_DOWN  && g_pro.gpower_on == power_on){
        xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                UP_BIT_3,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
         }
        
    ///    ENABLE_INT();
   break;



    }
}
