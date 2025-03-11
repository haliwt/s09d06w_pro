/*
 * bsp_wifi.c
 *
 *  Created on: 2025年3月7日
 *      Author: Administrator
 */
#include "bsp.h"

wifi_state_ref g_wifi;

uint8_t wifi_rx_inputBuf[1];

uint8_t first_connect_wifi_flag;

static void link_wifi_net_handler(void);


/********************************************************************************
	*
	*Functin Name:void link_wifi_to_tencent_handler(uint8_t data)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void link_wifi_to_tencent_handler(uint8_t data)
{
    if(data == 1){//if(gpro_t.wifi_led_fast_blink_flag==1){
        if(g_wifi.gTimer_wifi_led_fast_blink  > 119){

           g_wifi.gTimer_wifi_led_fast_blink =0;//
           
           if(g_wifi.gwifi_link_net_state_flag==0){

              g_wifi.wifi_get_beijing_step = 10; //auto connect wifi net.
              g_wifi.gTimer_auto_detected_net_state_times = 120;
              g_wifi.linking_tencent_cloud_doing =1;
              g_wifi.soft_ap_config_flag =1; //WE.EIDT 
         
           }

    }
    else{
	   

       link_wifi_net_handler();
    }
    }
        

   if(g_wifi.gwifi_link_net_state_flag ==1 && first_connect_wifi_flag ==1){
         first_connect_wifi_flag++;
         Update_Dht11_Totencent_Value();
         osDelay(20);//HAL_Delay(200) //WT.EDIT 2024.08.10
     }

	
       
  
}


/*****************************************************************************
    *
    *Function Name: static void AutoReconnect_Wifi_Neware_Function(void)
    *Function: 
    *Input Ref: NO
    *Return Ref:NO
    *
*****************************************************************************/	 	
static void link_wifi_net_handler(void)
{
  

    uint8_t  device_massage[100];
  // device_massage = (uint8_t *)malloc(128);

      

        switch(g_wifi.link_net_step){

            case 0: //one step

          
        		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
        		//HAL_Delay(1000);
        		osDelay(1000);

                 g_wifi.link_net_step = 1;

            break;

            case 1:
               // WIFI_IC_ENABLE();
                HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);

                 randomName[0]=HAL_GetUIDw0();
                 g_wifi.gTimer_link_net_timer_time = 0;

                g_wifi.link_net_step = 2;

            break;

            case 2:
                 if(g_wifi.gTimer_link_net_timer_time  > 5){
                     g_wifi.gTimer_link_net_timer_time = 0;

                        // WIFI_IC_ENABLE();
            			
                        sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"UYIJIA01-%d\"\r\n", PRODUCT_ID, DEVICE_SECRET,randomName[0]);
            			at_send_data(device_massage, strlen((const char *)device_massage));
            	  		osDelay(1000);//HAL_Delay(1000);
                    
                       g_wifi.link_net_step = 3;

                 }

            break;


            case 3:
                
            if(g_wifi.gTimer_link_net_timer_time  > 7){
                    g_wifi.gTimer_link_net_timer_time = 0;
                   g_wifi.link_net_step = 4;
           // WIFI_IC_ENABLE();
			
            HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 0xffff); //动态注册 
	  		osDelay(1000);//HAL_Delay(1000);
        
          
            }

            break;


            case 4:
                 if(g_wifi.gTimer_link_net_timer_time  > 7){
                     g_wifi.gTimer_link_net_timer_time = 0;

                   g_wifi.linking_tencent_cloud_doing =1;
                 g_wifi.soft_ap_config_flag =1; //WE.EIDT 
	            sprintf((char *)device_massage, "AT+TCSAP=\"UYIJIA01-%d\"\r\n",randomName[0]);
                 at_send_data(device_massage, strlen((const char *)device_massage));


                   g_wifi.link_net_step = 5;


                    }

            break;


            case 5:
                

            if(g_wifi.soft_ap_config_success==1){

             g_wifi.soft_ap_config_success=0;
            HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
            //HAL_Delay(1000);
            // HAL_Delay(1000);
            ///HAL_Delay(1000);

            g_wifi.link_net_step = 6;
            g_wifi.gTimer_link_net_timer_time = 0;
            }

                   
            break;

            case 6:

            if( g_wifi.gTimer_link_net_timer_time  > 6){

             if(g_wifi.gwifi_link_net_state_flag==1){
			
			    
				//g_wifi.first_link_tencent_cloud_flag =1;
				first_connect_wifi_flag =1 ;
				g_wifi.get_rx_beijing_time_enable=0;
                
               SendWifiData_To_Data(0x1F,0x01); //link wifi order 1 --link wifi net is success.
			   g_wifi.link_net_step = 7;
              
				
		     }
		     else{
                
                  g_wifi.wifi_led_fast_blink_flag=0;
                  g_wifi.link_net_step = 8;
                  SendWifiData_To_Data(0x1F,0x00) ;	 //Link wifi net is fail .WT.EDTI .2024.08.31
                
           
                }
                
               }

            break;

            case 7:

              
              g_wifi.gTimer_get_data_from_tencent_data=0;
			 
				 MqttData_Publish_SetOpen(0x01);
		         HAL_Delay(20);
		        // osDelay(100);
		         Publish_Data_ToTencent_Initial_Data();
				 HAL_Delay(20);
                  //osDelay(100);

				Subscriber_Data_FromCloud_Handler();
				HAL_Delay(20);
	             //osDelay(100);
			 g_wifi.wifi_led_fast_blink_flag=0;

			 g_wifi.link_net_step = 0xfe;

                   
            break;


            case 8:


              g_wifi.wifi_get_beijing_step= 10;
              g_wifi.gTimer_auto_detected_net_state_times = 120;
              g_wifi.link_net_step = 0xff;

            break;


            default:


            break;


        }

}






