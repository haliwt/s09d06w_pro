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
	//const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500); /* 设置最大等待时间为30ms */
	uint32_t ulValue;


    while(1)
    {


	xResult = xTaskNotifyWait(0x00000000,
								  0xFFFFFFFF,     /* Reset the notification value to 0 on */
								&ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
								portMAX_DELAY);//portMAX_DELAY);  /* 阻塞时间30ms，释放CUP控制权,给其它任务执行的权限*/

		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */

			if((ulValue & DECODER_BIT_9) != 0){
				gl_tMsg.disp_rx_cmd_done_flag = 0;

				check_code =  bcc_check(gl_tMsg.usData,gl_tMsg.ulid);

				if(check_code == gl_tMsg.bcc_check_code ){

				 receive_data_fromm_display(gl_tMsg.usData);
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
*   priority: 2  (数值越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskRunPro(void *pvParameters)
{
  
   
    static uint8_t power_on_flag;

    while(1)
    {
      
        
   
          if(g_key.key_power_flag == KEY_POWER_ID){

		        power_on_key_counter ++ ;
            
		    if(KEY_POWER_VALUE() ==KEY_UP && power_on_key_counter < 100){
				g_key.key_power_flag=0;
				power_on_key_counter=0;
			    buzzer_sound();
                if(g_pro.gPower_on == power_off){
					g_pro.gPower_on = power_on;
				//gpro_t.send_ack_cmd = check_ack_power_on;
				//gpro_t.gTimer_again_send_power_on_off =0;
				//SendData_PowerOnOff(1);//power on
				}
				else{

				g_pro.gPower_on = power_off;
				//gpro_t.send_ack_cmd =  check_ack_power_off;
				//gpro_t.gTimer_again_send_power_on_off =0;
				//SendData_PowerOnOff(0);//power off
				

				}
		    }
			else if(KEY_POWER_VALUE() ==KEY_DOWN && power_on_key_counter  > 100){
		            g_key.key_power_flag=0;
				    power_on_key_counter=0;
			        g_key.key_long_power_flag =  KEY_LONG_POWER; //wifi led blink fast .
			        g_pro.gTimer_wifi_led_fast_blink = 0; //look for wifi information 120s,timer.
			
					buzzer_sound();



		   }
		}
		else if(g_key.key_mode_flag == KEY_MODEL_ID ){ //&& MODEL_KEY_VALUE()==KEY_UP){

		        mode_key_counter++ ;
		        if(KEY_MODE_VALUE() == KEY_UP && mode_key_counter < 6){
					g_key.key_mode_flag = KEY_NULL;
					mode_key_counter=0;
				    buzzer_sound();
				    //mode_key_fun();
		        }
				else if(KEY_MODE_VALUE() == KEY_DOWN && mode_key_counter > 5){

                    g_key.key_mode_flag = KEY_NULL;
					mode_key_counter=0;
					buzzer_sound();
                    //g_key.key_long_mode_flag = KEY_LONG_MODE; // input set up timer timing mode .
                    g_pro.gtime_timer_define_flag = timer_time_mode;

				
				}
		}
		else if(g_key.key_down_flag ==KEY_DOWN_ID){// && DEC_KEY_VALUE()==KEY_UP){
				g_key.key_down_flag ++;
				buzzer_sound();
				//SendData_Buzzer_Has_Ack();
				//gpro_t.send_ack_cmd = check_ack_buzzer;
				//gpro_t.gTimer_again_send_power_on_off =0;
				//key_dec_fun();
		}
		else if(g_key.key_up_flag ==1){ // && ADD_KEY_VALUE()==KEY_UP){
				g_key.key_up_flag ++;
				buzzer_sound();
				//SendData_Buzzer_Has_Ack();
				//gpro_t.send_ack_cmd = check_ack_buzzer;
				//gpro_t.gTimer_again_send_power_on_off =0;
				//key_add_fun();
		}
			
		

	  switch(g_pro.gPower_on){	

	   case power_on :

		// 检查LED硬件测试
		//Check_LED_Hardware_Test();

        // 只有在不进行LED测试时才执行正常的power_on处理
        if(!Is_LED_Testing())
        {
            power_on_run_handler();
        }
        break;

	  case power_off:
  
         if(power_on_flag==0){
             power_on_flag ++;
			 buzzer_sound();
		 }
         power_off_run_handler();

	   break;

      }
	 

     // send_cmd_ack_hanlder();

	  vTaskDelay(20);
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
    //const TickType_t xMaxBlockTime = pdMS_TO_TICKS(1000); /* 设置最大等待时间为30ms */
	uint32_t ulValue;
   

    while(1)
    {

      xResult = xTaskNotifyWait(0x00000000,
						           0xFFFFFFFF,
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
								  portMAX_DELAY);  /* 最大允许延迟时间 */
        if( xResult == pdPASS ){

            /* 接收到消息，检测那个位被按下 */
            if((ulValue & POWER_BIT_0 ) != 0)
            {
           
                  g_key.key_power_flag = KEY_POWER_ID;

            }
            else if((ulValue & MODE_BIT_1 ) != 0){   /* 接收到消息，检测那个位被按下 */
            	 if(g_pro.gPower_on == power_on){
            	     g_key.key_mode_flag = KEY_MODEL_ID;

            	 }

             }
            else if((ulValue & DOWN_BIT_2 ) != 0){
            	  if(g_pro.gPower_on == power_on){
            	       g_key.key_down_flag =KEY_DOWN_ID;
            	             
            	  }

            }
            else if((ulValue & UP_BIT_3 ) != 0){   /* 接收到消息，检测那个位被按下 */
            	 if(g_pro.gPower_on == power_on){
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


    if(huart==&huart1) // Motor Board receive data (filter)
	{

    //   DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0x5A){  // 0x5A --main board singla
               gl_tMsg.rx_data_counter=0;
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


                xTaskNotifyFromISR(xHandleTaskDecoderPro,  /* 目标任务 */
                                    DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                                    eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                                    &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

              }

              }

              if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE && rx_end_flag == 0 &&   gl_tMsg.rx_data_counter > 4){

                     rx_end_flag = 1 ;

              }

        break;



		}

       //   ENABLE_INT();
    __HAL_UART_CLEAR_OREFLAG(&huart1);
	HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte

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

       if(KEY_MODE_VALUE() == KEY_DOWN && g_pro.gPower_on == power_on){
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
       if(KEY_DOWN_VALUE() == KEY_DOWN && KEY_UP_VALUE() == KEY_UP && g_pro.gPower_on == power_on){
        
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
        if(KEY_UP_VALUE() == KEY_DOWN && KEY_DOWN_VALUE() == KEY_UP && g_pro.gPower_on == power_on){
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
