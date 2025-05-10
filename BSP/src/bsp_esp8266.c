/*
 * bsp_esp8266.c
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */
#include "bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define  DELAY_TIME_MS   1000


 uint8_t *sub_buf;

char *CloudInfo="+TCMQTTCONN:OK";
char *usart2_tx;

uint8_t usart2_flag;


uint32_t randomName[1];
static uint8_t device_massage[128];




/**
 *pdata: pointer of data for send
 *len:  len of data to be sent
 *return: the len of data send success
 * @brief hal api for at data send
 */
uint8_t at_send_data(uint8_t* pdata, uint16_t len)
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, pdata, len, 10000))
	{
		return len;
	}
	else
	{
		return 0;
	}	
}


void RTOS_Delay(uint32_t ms) {
    for (uint32_t i = 0; i < ms / DELAY_TIME_MS; i++) {
        osDelay(DELAY_TIME_MS);
    }
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void InitWifiModule(void)
{
	
	if(g_wifi.wifi_config_net_lable==0){
		 g_wifi.wifi_config_net_lable++;
	    at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		//HAL_Delay(1000);
		osDelay(1000);
	}
		
}

void InitWifiModule_Hardware(void)
{
	
	at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
	//HAL_Delay(1000);
	osDelay(1000);
			
		
}

void ReConnect_Wifi_Net_ATReset_Hardware(void)
{
	   
		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		//HAL_Delay(1000);
		osDelay(1000);

}



/****************************************************************************************************
	**
	*Function Name:void Wifi_SoftAP_Config_Handler(void)
	*Function: tensent cloud configuration softAP to connector WIFI
	*Input Ref: 
	*Return Ref:NO
	*
****************************************************************************************************/
void Wifi_SoftAP_Config_Handler(void)
{
     //uint8_t *device_massage;

	// device_massage = (uint8_t *)malloc(128);

	
    

  


   switch (g_wifi.wifi_config_net_lable)
  {

    case wifi_set_restor:
           //InitWifiModule();
           ReConnect_Wifi_Net_ATReset_Hardware();//InitWifiModule_Hardware();
		   //HAL_Delay(1000);
		 //  osDelay(1000);
           g_wifi.wifi_config_net_lable =wifi_set_cwmode;
	break;


	 case wifi_set_cwmode:
    	   // WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(1000);
           
			//HAL_Delay(1000);
           // osDelay(1000);
           /// osDelay(1000);
			//HAL_UART_Transmit(&huart2, "AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 5000);
			g_wifi.wifi_config_net_lable =wifi_set_softap;
			randomName[0]=HAL_GetUIDw0();
		

	 break;

	  case wifi_set_softap:
           // WIFI_IC_ENABLE();
			
            sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"UYIJIA01-%d\"\r\n", PRODUCT_ID, DEVICE_SECRET,randomName[0]);
			usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
	  		HAL_Delay(1000);
        
          
			osDelay(1000);
            osDelay(1000);
       		osDelay(1000);
           	osDelay(1000);
         
	        
			g_wifi.wifi_config_net_lable=wifi_set_tcdevreg;
		
	


	 case wifi_set_tcdevreg://dynamic register
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 0xffff); //动态注册 
	      HAL_Delay(1000);
      

	    
   
		osDelay(1000);
        osDelay(1000);

	  
	     g_wifi.wifi_config_net_lable=wifi_set_tcsap;

	 break;


	 case wifi_set_tcsap:
	 
   
           osDelay(1000);
           osDelay(1000);
           osDelay(1000);
           osDelay(1000);
           osDelay(1000);
           osDelay(1000);
		  
         
            g_wifi.linking_tencent_cloud_doing =1;
          
	        sprintf((char *)device_massage, "AT+TCSAP=\"UYIJIA01-%d\"\r\n",randomName[0]);
            usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
		
           osDelay(1000);
           osDelay(1000);
            HAL_Delay(1000);
           HAL_Delay(1000);
         
			g_wifi.wifi_config_net_lable=0xff;//
		  //   gctl_t.wifi_config_net_lable=wifi_inquire_register_codes;
	break;
          
	case wifi_inquire_register_codes: //0x06//WT.EDIT 2024.07.22
       g_wifi.gTimer_get_beijing_time=0;

         osDelay(1000);
         osDelay(1000);
           osDelay(1000);
       
	   if(g_wifi.soft_ap_config_success==0){
         
		// net_t.wifi_uart_counter=0;
        HAL_UART_Transmit(&huart2, "AT+TCPRDINFOSET?\r\n", strlen("AT+TCPRDINFOSET?\r\n"), 0xffff); //动
        HAL_Delay(1000);
        g_wifi.wifi_config_net_lable=0xff;//

        }

	 }

  //free(device_massage);
}

/****************************************************************************************************
**
*Function Name:void Wifi_Link_SmartPhone_Fun(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void SmartPhone_LinkTencent_Cloud(void)
{
   
    if(g_wifi.soft_ap_config_success==1){

       g_wifi.soft_ap_config_success=0;
	   HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
       HAL_Delay(1000);
       ///HAL_Delay(1000);
       
	 
     }
	
 	

}



void wifi_Disconnect_Fun(void)
{

  HAL_UART_Transmit(&huart2, "AT+TCMQTTDISCONN\r\n", strlen("AT+TCMQTTDISCONN\r\n"), 5000);//开始连接

}

void Get_BeiJing_Time_Cmd(void)
{

 // HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n", strlen("AT+CIPSNTPCFG=1,800,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"), 5000);//开始连接
   HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8\r\n", strlen("AT+CIPSNTPCFG=1,8\r\n"), 0xffff);//开
}

void Get_Beijing_Time(void)
{

   HAL_UART_Transmit(&huart2, "AT+CIPSNTPTIME?\r\n", strlen("AT+CIPSNTPTIME?\r\n"), 0xffff);//开始连接


}

void PowerOn_Self_Auto_Link_Tencent_Cloud(void)
{

	uint8_t auto_link_cloud_flag=0;
    

    //device_massage = (uint8_t *)malloc(128);


   switch (auto_link_cloud_flag)
  {

    case 0:
           InitWifiModule_Hardware();//InitWifiModule();
           
		   HAL_Delay(1000);
		   
           auto_link_cloud_flag =wifi_set_cwmode;
	break;


	 case wifi_set_cwmode:
    	   // WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
     
            HAL_Delay(1000);
	
			randomName[0]=HAL_GetUIDw0();
		

	 break;
	 default:
        
	 break;

   }
  //free(device_massage);

}

void SmartPhone_TryToLink_TencentCloud(void)
{
    g_wifi.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
    g_wifi.wifi_rx_data_counter=0;

    HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接

	HAL_Delay(1000);
    HAL_Delay(1000);
	HAL_Delay(1000);
	
	   
}


void Reconnection_Wifi_Order(void)
{
	HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 0xffff);
    HAL_Delay(200);
  
}














