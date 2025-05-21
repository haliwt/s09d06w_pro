/*
 * bsp_phone_decoder.c
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */
#include "bsp.h"


uint8_t TCMQTTRCVPUB[40];





typedef struct{

  uint8_t received_data_from_tencent_cloud;
  uint8_t response_wifi_signal_label;
  uint8_t rx_counter;
  uint8_t  rx_data_state; 
  uint8_t rx_data_success;
  

}local_ref_t;

local_ref_t gl_msg;

uint8_t test_flag;
//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/


 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
          // uint8_t *device_massage;

         //device_massage = (uint8_t *)malloc(128);

		  uint8_t device_massage[128];
         randomName[0]=HAL_GetUIDw0();
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID,randomName[0]);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000); 
        // free(device_massage);
}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{
     
    switch(gl_msg.rx_data_state)
      {
      case 0: //#0

	  	if((wifi_rx_inputBuf[0] == 'p') ||wifi_rx_inputBuf[0]=='E' || wifi_rx_inputBuf[0] =='T') //hex :54 - "T" -fixed
            gl_msg.rx_data_state=1; //=1
          else{
               gl_msg.rx_counter=0;
               gl_msg.rx_data_state=0; 
            }
        break;

      case 1:
      
         if((wifi_rx_inputBuf[0] == 'a')  ||wifi_rx_inputBuf[0]=='R' || wifi_rx_inputBuf[0] =='C')//hex :54 - "T" -fixed
            gl_msg.rx_data_state=2; //=1
          else{
               gl_msg.rx_counter=0;
               gl_msg.rx_data_state=1;
            }
            
         break;
      case 2: //#1
             if((wifi_rx_inputBuf[0] == 'r')||wifi_rx_inputBuf[0]=='R'|| wifi_rx_inputBuf[0] =='M')  //hex :4B - "K" -fixed
            gl_msg.rx_data_state=3; //=1
         else{
            gl_msg.rx_data_state =0;
             gl_msg.rx_counter=0;
         }
         break;
            
        case 3:
            if((wifi_rx_inputBuf[0] == 'a')||wifi_rx_inputBuf[0]=='o'|| wifi_rx_inputBuf[0] =='Q')    //hex :4B - "K" -fixed
            gl_msg.rx_data_state=4; //=1
         else{
           gl_msg.rx_data_state =0;
             gl_msg.rx_counter=0;
         }
        
        break;
        
        case 4:
            if((wifi_rx_inputBuf[0] == 'm')  ||wifi_rx_inputBuf[0]=='R'|| wifi_rx_inputBuf[0] =='T')  //hex :4B - "K" -fixed
            gl_msg.rx_data_state=5; //=1
         else{
            gl_msg.rx_data_state =0;
             gl_msg.rx_counter=0;
         }
        
        break;

      case 5:
       if((wifi_rx_inputBuf[0] == 's') ||wifi_rx_inputBuf[0]==':'|| wifi_rx_inputBuf[0] =='T')   //hex :4B - "K" -fixed
         gl_msg.rx_data_state=6; //=1
         else{
           gl_msg.rx_data_state=0;
            gl_msg.rx_counter=0;
         }
            
      break;

      
      case 6:
       if((wifi_rx_inputBuf[0] == '"')||wifi_rx_inputBuf[0]=='2' || wifi_rx_inputBuf[0] =='S')    //hex :4B - "K" -fixed
         gl_msg.rx_data_state=7; //=1
         else{
           gl_msg.rx_data_state =0;
            gl_msg.rx_counter=0;
         }
            
      break;

      case 7:
       if((wifi_rx_inputBuf[0] == ':')||wifi_rx_inputBuf[0]=='0' ||wifi_rx_inputBuf[0]=='T' ){  //hex :4B - "K" -fixed
         gl_msg.rx_data_state=8; //=1
    	}
		else{
           gl_msg.rx_data_state =0;
            gl_msg.rx_counter=0;
         }
           
      break;

       case 8:
       if((wifi_rx_inputBuf[0] == '{') ||wifi_rx_inputBuf[0]=='8' ||wifi_rx_inputBuf[0]=='A' ){ //hex :4B - "K" -fixed
         if(wifi_rx_inputBuf[0]=='8')  gl_msg.rx_data_state =11;
         else if(wifi_rx_inputBuf[0]=='A')  gl_msg.rx_data_state =12;
         else
             gl_msg.rx_data_state =9; //=1

        }
         else{
           gl_msg.rx_data_state =0;
            gl_msg.rx_counter=0;
         }

		
            
      break;


      case 9:

       if(gl_msg.rx_data_success ==0){

            g_wifi.wifi_rx_data_array[gl_msg.rx_counter] = wifi_rx_inputBuf[0];
            gl_msg.rx_counter++ ;

            if(wifi_rx_inputBuf[0]=='}' || wifi_rx_inputBuf[0]==0x0A) //0x7D='}', 0x0A = line feed // end
            {
                gl_msg.rx_data_success=1;
                gl_msg.rx_data_state=0;
                gl_msg.received_data_from_tencent_cloud = gl_msg.rx_counter;
                gl_msg.rx_counter=0;


            }


       }
       else{
            gl_msg.rx_data_success=0;
            gl_msg.rx_data_state =0;
            gl_msg.rx_counter=0;
            gl_msg.received_data_from_tencent_cloud =0;


       }

     break;

     
      case 11:
		 g_wifi.gwifi_link_net_state_flag= 0;//net_t.wifi_link_net_success =0; //wifi_t.esp8266_login_cloud_success =0;
         g_wifi.get_beijing_time_success = 0;
         gl_msg.rx_data_state =0;
         gl_msg.rx_counter=0;

      break;

      case 12:
		if(wifi_rx_inputBuf[0]== 'T'){   //hex :4B - "K" -fixed
         gl_msg.rx_data_state=13; //=1
        }
         else{
            gl_msg.rx_data_state =0;
            gl_msg.rx_counter=0;
         }

      break;

	  case 13:

       if(wifi_rx_inputBuf[0]== 'E'){   //hex :4B - "K" -fixed
          gl_msg.rx_data_state=14; //=1
        }
        else{
             gl_msg.rx_data_state =0;
             gl_msg.rx_counter=0;
         }

      break;

	  case 14:
		if(wifi_rx_inputBuf[0]== ':'){   //hex :4B - "K" -fixed
          gl_msg.rx_data_state=15; //=1
         }
         else{
             gl_msg.rx_data_state =0;
             gl_msg.rx_counter=0;
         }

      break;

	  case 15:
		if(wifi_rx_inputBuf[0]== '0'){   //hex :4B - "K" -fixed
            g_wifi.gwifi_link_net_state_flag = 0;//wifi_t.esp8266_login_cloud_success =0;
            g_wifi.get_beijing_time_success = 0;
            gl_msg.rx_data_state =0;
            gl_msg.rx_counter=0;
        }
        else if(wifi_rx_inputBuf[0]== '1'){

            //net_t.wifi_link_net_success = 1;//net_t.esp8266_login_cloud_success =1;
             g_wifi.gwifi_link_net_state_flag = 1;
             gl_msg.rx_data_state =0;
             gl_msg.rx_counter=0;
        }
        else{

           gl_msg.rx_data_state =0;
           gl_msg.rx_counter=0;


        }

      break;

	

	  default:
      break;
      }
 

}
 
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
  if(strstr((const char*)g_wifi.wifi_rx_data_array,"+TCSAP:WIFI_CONNECT_SUCCESS")){
    g_wifi.soft_ap_config_success=1;

  }
  else if(strstr((const char*)g_wifi.wifi_rx_data_array,"+TCMQTTCONN:OK")){
    //net_t.wifi_link_net_success=1;
    g_wifi.gwifi_link_net_state_flag=1;
    g_wifi.linking_tencent_cloud_doing=0;
    g_wifi.auto_link_cloud_flag=0xff;
    g_wifi.soft_ap_config_flag=0;

   
  }
  else if(strstr((char*)g_wifi.wifi_rx_data_array,"+CME ERROR:208")){

    g_wifi.linking_tencent_cloud_doing=0; //release this flag. usart
    //net_t.wifi_link_net_success=0;
     g_wifi.gwifi_link_net_state_flag=0;
  
      g_wifi.wifi_led_fast_blink_flag=0; 

   
  }
  else if(strstr((char*)g_wifi.wifi_rx_data_array,"+TCPRDINFOSET:1,\"EHQB1P53IH\"")){ //WT.EDIT 2024.07.22

  // wifi_t.serch_for_wifi_flag=1;
  //  wifi_t.gTime_link_time_start =0;

  }
  else  if(strstr((char*)g_wifi.wifi_rx_data_array,"+TCMQTTCONN:FAIL,202")){

    //  wifi_t.esp8266_login_cloud_success =0;
   // net_t.wifi_link_net_success=0;
    g_wifi.gwifi_link_net_state_flag=0;
    g_wifi.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31

    //wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    g_wifi.linking_tencent_cloud_doing=0;
    g_wifi.get_rx_beijing_time_enable=0;
	g_wifi.soft_ap_config_flag=0xff;
  }
  else if(strstr((const char*)g_wifi.wifi_rx_data_array,"+TCMQTTCONN:OK")){
    //net_t.wifi_link_net_success=1;
     g_wifi.gwifi_link_net_state_flag=1;
    g_wifi.linking_tencent_cloud_doing=0;
    g_wifi.auto_link_cloud_flag=0xff;

    g_wifi.soft_ap_config_flag=0;
  }
  else if(strstr((char*)g_wifi.wifi_rx_data_array,"+CME ERROR:208")){


   // net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
    g_wifi.gwifi_link_net_state_flag=0;
    g_wifi.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    g_wifi.get_rx_beijing_time_enable=0;

  }
  else if(strstr((char*)g_wifi.wifi_rx_data_array,"+TCMQTTCONN:FAIL,202")){

    //net_t.wifi_link_net_success=0; //wifi_t.esp8266_login_cloud_success =0;
    g_wifi.gwifi_link_net_state_flag=0;
    g_wifi.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31

    g_wifi.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart

    g_wifi.get_rx_beijing_time_enable=0;
  }
  else if(strstr((char*)g_wifi.wifi_rx_data_array,"+TCMQTTDISCON")){

   // net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
    g_wifi.gwifi_link_net_state_flag=0;
    g_wifi.get_rx_beijing_time_enable=0;
    g_wifi.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
  }
  else if(strstr((char*)g_wifi.wifi_rx_data_array,"+TCSAP:WIFI_CONNECT_FAILED")){

   // net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
    g_wifi.gwifi_link_net_state_flag=0;
    g_wifi.get_rx_beijing_time_enable=0;
    g_wifi.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    }

  g_wifi.wifi_rx_data_counter=0;
  g_wifi.once_rx_data_done = 1;
         
}


void clear_rx_copy_data(void)
{
   //uint8_t i;
   #if 0
   if( wifi_t.once_rx_data_done == 1 && wifi_link_net_state() == 0){

      wifi_t.once_rx_data_done ++;

      for(i=0;i<150;i++){
        
       wifi_t.wifi_data[i]=0;
      // wifi_t.auto_det_data[i]=0;
      }
    }
   #endif 
   if( g_wifi.once_rx_data_done == 1){
      g_wifi.once_rx_data_done ++; //WT.EDIT 2024.08.17
      memset(g_wifi.wifi_rx_data_array,'\0',150);
    }
}

/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{
    
   
    if(gl_msg.rx_data_success==1){
            gl_msg.rx_data_success=0;
        
	    
	if(gl_msg.received_data_from_tencent_cloud > 22){
	    gl_msg.received_data_from_tencent_cloud=0;
		g_wifi.get_rx_beijing_time_enable=0;
		gl_msg.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
         
	   // __HAL_UART_CLEAR_OREFLAG(&huart2);
		strncpy((char*)TCMQTTRCVPUB,(char *)g_wifi.wifi_rx_data_array,40);
	    
	
	}
	else{
	if(strstr((char *)g_wifi.wifi_rx_data_array,"open\":0")){
		  gl_msg.response_wifi_signal_label = OPEN_OFF_ITEM;
		 
	}
	else if(strstr((char *)g_wifi.wifi_rx_data_array,"open\":1")){
	   
	   gl_msg.response_wifi_signal_label = OPEN_ON_ITEM;
	}


	
	if(strstr((char *)g_wifi.wifi_rx_data_array,"ptc\":0")){
            if(g_pro.gpower_on ==power_on){
				 // dry_open_flag=0;//gctl_t.gDry=0;
                 g_pro.gDry = 0;
				 LED_DRY_OFF();
	           gl_msg.response_wifi_signal_label = PTC_OFF_ITEM;
	         
             }
			
    }
    else if(strstr((char *)g_wifi.wifi_rx_data_array,"ptc\":1")){
            if(g_pro.gpower_on ==power_on){
	          //dry_open_flag=1;//gctl_t.gDry=1;
              g_pro.gDry = 1;
			  LED_DRY_ON();
			 gl_msg.response_wifi_signal_label = PTC_ON_ITEM;
				
            }

    }
	
    if(strstr((char *)g_wifi.wifi_rx_data_array,"Anion\":0")){
          if(g_pro.gpower_on ==power_on){
	          //  gctl_t.gPlasma=0;
	          g_pro.gPlasma = 0;
			  LED_PLASMA_OFF();
			  
			gl_msg.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
		 
    }
    else if(strstr((char *)g_wifi.wifi_rx_data_array,"Anion\":1")){
            if(g_pro.gpower_on ==power_on){
            //gctl_t.gPlasma=1;
             g_pro.gPlasma =1;
			 LED_PLASMA_ON();
			gl_msg.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
    }
	
    if(strstr((char *)g_wifi.wifi_rx_data_array,"sonic\":0")){
            if(g_pro.gpower_on ==power_on){
           // gctl_t.gUlransonic=0;
             g_pro.gMouse = 0;
		     LED_MOUSE_ON();
			gl_msg.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
		
    }
    else if(strstr((char *)g_wifi.wifi_rx_data_array,"sonic\":1")){
            if(g_pro.gpower_on ==power_on){
            //ultrasonic_open_flag=1;//gctl_t.gUlransonic=1;
             g_pro.gMouse = 1;
			 LED_MOUSE_OFF();
			gl_msg.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
			
    }

	
    if(strstr((char *)g_wifi.wifi_rx_data_array,"state\":1")){
           if(g_pro.gpower_on ==power_on){
            g_pro.gAI=1;
			LED_AI_ON();
			gl_msg.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
        	}
		  
    }
    else if(strstr((char *)g_wifi.wifi_rx_data_array,"state\":2")){
            if(g_pro.gpower_on ==power_on){
            g_pro.gAI=2;
			LED_AI_OFF();
			gl_msg.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
            }
			
    }
	
    if(strstr((char *)g_wifi.wifi_rx_data_array,"temperature")){

	        if(g_pro.gpower_on ==power_on){
			gl_msg.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
			
    }
   if(strstr((char *)g_wifi.wifi_rx_data_array,"find")){

		 if(g_pro.gpower_on ==power_on){

			gl_msg.response_wifi_signal_label= FAN_ITEM;
		}
	}
 
    }
    }
 }

/****************************************************************************
*****************************************************************************/
void Json_Parse_Command_Fun(void)
{

 
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
	static uint8_t buzzer_temp_on;


   switch(gl_msg.response_wifi_signal_label){
  
	case OPEN_ON_ITEM:
      
		MqttData_Publish_SetOpen(1);  
		HAL_Delay(100);//osDelay(100);//HAL_Delay(100);

        Publish_Data_ToTencent_Initial_Data();
	    HAL_Delay(200);

		g_pro.ptc_warning =0;
		
         g_pro.gpower_on = power_on;
  
	     SendWifiData_To_Cmd(0x31,0x01); //smart phone is power on
         osDelay(5);

       
		buzzer_temp_on=0;
		gl_msg.response_wifi_signal_label = 0xff;

	  break;

       case OPEN_OFF_ITEM:

      
		 	MqttData_Publish_SetOpen(0);  
			osDelay(100);

            g_pro.gpower_on = power_off;
	
            SendWifiData_To_Cmd(0x31,0x0); //smart phone is power off
			osDelay(5);//HAL_Delay(5);
          
			buzzer_temp_on=0;
	
         
        gl_msg.response_wifi_signal_label = 0xff;
        
	  break;

	  case PTC_ON_ITEM:
	  if(g_pro.gpower_on ==power_on){
	    if(g_pro.ptc_warning ==0){
         MqttData_Publish_SetPtc(0x01);
	  	 osDelay(50);//HAL_Delay(350);
		 g_pro.gDry = 1;
		 LED_DRY_ON();

		 if(g_pro.works_two_hours_interval_flag==0){
		 	LED_DRY_ON();
		    DRY_OPEN();

		 }
	     g_pro.g_manual_shutoff_dry_flag = 0;
         if(g_disp.g_second_disp_flag == 1){
			 SendWifiData_To_Cmd(0x02,0x01);
			 osDelay(5);//HAL_Delay(5);
         }
		
         }
         }
		 buzzer_temp_on=0;
         gl_msg.response_wifi_signal_label=0xff;
	  	
       
	   break;

	  case PTC_OFF_ITEM:
	  	if(g_pro.gpower_on ==power_on){
	
         MqttData_Publish_SetPtc(0);
		 osDelay(100);//HAL_Delay(350);
		 g_pro.gDry = 0;
		 LED_DRY_OFF();
	     DRY_CLOSE();
		 
		 g_pro.g_manual_shutoff_dry_flag = 1;
		  if(g_disp.g_second_disp_flag == 1){
			 SendWifiData_To_Cmd(0x02,0x0);
	         osDelay(5);//HAL_Delay(5);
		  }
         }
		buzzer_temp_on=0;
	     gl_msg.response_wifi_signal_label = 0xff;

	  	
	  	break;

	  case ANION_OFF_ITEM: //"杀菌" //5
	  	if(g_pro.gpower_on ==power_on){
			
            MqttData_Publish_SetPlasma(0);
			osDelay(100);//HAL_Delay(350);
			g_pro.gPlasma =0 ;
			PLASMA_CLOSE();
			LED_PLASMA_OFF();
			if(g_disp.g_second_disp_flag == 1){
			SendWifiData_To_Cmd(0x03,0x0);
	  	    osDelay(5);//HAL_Delay(5);
			 }
	  	}
      
		buzzer_temp_on=0;
	   gl_msg.response_wifi_signal_label = 0xff;
	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(g_pro.gpower_on ==power_on){
            MqttData_Publish_SetPlasma(1);
			osDelay(100);//HAL_Delay(350);
		      g_pro.gPlasma =1 ;
              LED_PLASMA_ON();
              if(g_disp.g_second_disp_flag == 1){
					SendWifiData_To_Cmd(0x03,0x01);
	  	   			osDelay(5);//HAL_Delay(5);
              }
	  	}
  
		buzzer_temp_on=0;
	   gl_msg.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(g_pro.gpower_on ==power_on){

            MqttData_Publish_SetUltrasonic(0);
			osDelay(100);	//HAL_Delay(350);
			g_pro.gMouse = 0;
			mouse_close();
			LED_MOUSE_OFF();
           
			  if(g_disp.g_second_disp_flag == 1){
			SendWifiData_To_Cmd(0x04,0x0);
			osDelay(5);
			  }
        }
		buzzer_temp_on=0;
	   gl_msg.response_wifi_signal_label=0xff;
	  	break;

	  case SONIC_ON_ITEM://ultransonic oN
	    if(g_pro.gpower_on ==power_on){
		
             MqttData_Publish_SetUltrasonic(1);
			 osDelay(100);	//HAL_Delay(350);

             g_pro.gMouse = 1;
			 LED_MOUSE_ON();

        
		    if(g_disp.g_second_disp_flag == 1){
			SendWifiData_To_Cmd(0x04,0x01);
			osDelay(5);//HAL_Delay(5);
		    }
        }
        
		buzzer_temp_on=0;
	   gl_msg.response_wifi_signal_label=0xff;
	  	break;

	  case STATE_TIMER_MODEL_ITEM: //timer timing mode 0x0A
	  if(g_pro.gpower_on ==power_on){

      
             g_pro.gAI=2;
			 LED_AI_OFF();
             MqttData_Publish_SetState(2);
    	     osDelay(100);//HAL_Delay(350);
            if(g_disp.g_second_disp_flag ==1){
    	        SendWifiData_To_Cmd(0x27,0x02);
				osDelay(5);
            }

           
        }
    
	    buzzer_temp_on=0;
	  gl_msg.response_wifi_signal_label = 0xff;
	  break;
		
	  case STATE_AI_MODEL_ITEM: // beijing timing 0x09
	  	 if(g_pro.gpower_on ==power_on){
		
              g_pro.gAI=1;
			  LED_AI_ON();
              MqttData_Publish_SetState(1);
    		  osDelay(100);//HAL_Delay(350);
            
    		   SendWifiData_To_Cmd(0x27,0x01);

            
        }
     
		buzzer_temp_on=0;
	    gl_msg.response_wifi_signal_label = 0xff;
	  	break;

	  case TEMPERATURE_ITEM:
	   if(g_pro.gpower_on ==power_on){
		

            temp_decade=g_wifi.wifi_rx_data_array[14]-0x30;
            temp_unit=g_wifi.wifi_rx_data_array[15]-0x30;
            g_wifi.wifi_set_temperature_value = temp_decade*10 +  temp_unit;
            if( g_wifi.wifi_set_temperature_value > 40)  g_wifi.wifi_set_temperature_value=40;
            if( g_wifi.wifi_set_temperature_value <20 )  g_wifi.wifi_set_temperature_value=20;
         
			
            g_pro.gset_temperture_value = g_wifi.wifi_set_temperature_value;
			
			g_wifi.g_wifi_set_temp_flag=1;
			g_pro.gTimer_input_set_temp_timer= 0;
			g_pro.g_manual_shutoff_dry_flag=0;
			
			
            MqttData_Publis_SetTemp(g_wifi.wifi_set_temperature_value);
		    osDelay(100);//HAL_Delay(350);
		    if(g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
			    TM1639_Display_Temperature(g_wifi.wifi_set_temperature_value); //WT.EDIT 2025.03.11
		    }
			if(g_disp.g_second_disp_flag == 1){
				SendWifiData_One_Data(0x2A, g_wifi.wifi_set_temperature_value); //smart phone set temperature value .
				osDelay(5);//HAL_Delay(10);
		    }
			
       }
     
	  buzzer_temp_on=0;
	  gl_msg.response_wifi_signal_label = 0xff;
	  break;

	  case FAN_ITEM:
	    if(g_pro.gpower_on ==power_on){

		     if(g_pro.fan_warning ==0){//if(gctl_t.fan_warning ==0){

           		 wind_hundred =g_wifi.wifi_rx_data_array[7]-0x30;
	       		 wind_decade=g_wifi.wifi_rx_data_array[8]-0x30;
                 wind_unit = g_wifi.wifi_rx_data_array[9]-0x30;
            
                if(wind_hundred ==1 && wind_decade==0 && wind_unit==0)g_wifi.set_wind_speed_value=100;
                else
                     g_wifi.set_wind_speed_value = wind_hundred*10 + wind_decade;
			
         
			MqttData_Publis_SetFan(g_wifi.set_wind_speed_value);
			osDelay(100);//HAL_Delay(350);
			if(g_disp.g_second_disp_flag == 1){
	    		SendWifiData_To_PanelWindSpeed(g_wifi.set_wind_speed_value);
				osDelay(5);//HAL_Delay(10);
			}
          
		    }
			else{
				g_wifi.set_wind_speed_value=0;

			    MqttData_Publis_SetFan(g_wifi.set_wind_speed_value);
				osDelay(100);//HAL_Delay(350);


			}
            
		}
     
	  	buzzer_temp_on=0;
	    gl_msg.response_wifi_signal_label = 0xff;
	  	break;

	  case APP_TIMER_POWER_ON_REF :

	  
		   g_wifi.get_rx_beijing_time_enable=0; //enable beijing times
	  	
		   if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		   
			  g_wifi.app_timer_power_on_flag = 1;
              
			  
			   SendWifiData_To_Cmd(0x21,0x01); //smart phone is open that App timer 
               osDelay(5);
			   
			  

		       g_pro.gpower_on = power_on; //WT.EDIT 
			   g_wifi.link_net_step = 0; //WT.EDIT 2025.05.12
			    buzzer_temp_on=0;
   

		         
           }
		   else if(strstr((char *)TCMQTTRCVPUB,"open\":0")){
		   
		   
			    g_wifi.app_timer_power_on_flag = 0;

		 		  MqttData_Publish_SetOpen(0);  
			       osDelay(100);//HAL_Delay(350);
	
	            g_pro.gpower_on = power_off;
            if(g_disp.g_second_disp_flag == 1){
			SendWifiData_To_Cmd(0x21,0x0); //turn off power off
			osDelay(10);//HAL_Delay(10);
            }
		      buzzer_temp_on=0;
				
			}

	     gl_msg.response_wifi_signal_label=0xff;

	  break;


   }


   if(gl_msg.response_wifi_signal_label==0xff){
        
        if(buzzer_temp_on ==0){
			buzzer_temp_on++;
   	        buzzer_sound();
        }
         
		

//		for(i=0;i<20;i++){
//		   gpro_t.wifi_rx_data_array[i]=0;
//		   
//
//        }

         memset(g_wifi.wifi_rx_data_array,'\0',20);
      
		gl_msg.response_wifi_signal_label=0xf0;
	}

  

  }

/********************************************************************
*
*Function Name: void Parse_Json_phone_timer_power_on_ref(void)
*Function :
*Input Ref:
*Return Ref:
*
********************************************************************/
void Parse_Json_phone_timer_power_on_ref(void)
{

   
    
    if(strstr((char *)TCMQTTRCVPUB,"sonic\":0")){
			
			g_pro.gMouse =0;    // ultrasonic_open_flag=0;//gctl_t.gUlransonic=0;
				
			
		}
		else if(strstr((char *)TCMQTTRCVPUB,"sonic\":1")){
			
				g_pro.gMouse =1;//ultrasonic_open_flag=1;//gctl_t.gUlransonic=1;
				
		   }
        
           
      if(strstr((char *)TCMQTTRCVPUB,"Anion\":0")){
			
				g_pro.gPlasma = 0;  // plasma_open_flag=0;//gctl_t.gPlasma=0;
				
				
			 
		}
		else if(strstr((char *)TCMQTTRCVPUB,"Anion\":1")){
			
				g_pro.gPlasma = 1; //plasma_open_flag=1;//gctl_t.gPlasma=1;
				
			
				
		}
        if(strstr((char *)TCMQTTRCVPUB,"ptc\":0")){
				
			g_pro.gDry = 0;//dry_open_flag=0;//gctl_t.gDry=0;
           
				  
		}
		else if(strstr((char *)TCMQTTRCVPUB,"ptc\":1")){
				
			g_pro.gDry = 1;  // dry_open_flag=1;//gctl_t.gDry=1;
                 
		 }
		
}





uint8_t  read_wifi_dry_value(void)
{
    return g_pro.g_manual_shutoff_dry_flag;

}

uint8_t read_wifi_temperature_value(void)
{
   if(g_wifi.g_wifi_set_temp_flag==1){
        
	
       return 1;
   }
   else{
       return 0;

   }
  
}

