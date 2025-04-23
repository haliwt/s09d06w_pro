/*
 * bsp_message.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

static void copy_receive_data(uint8_t cmd,uint8_t data);


uint8_t power_off_test_counter;
uint8_t temperature_value;

/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC校验函数
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}
/**********************************************************************
    *
    *Function Name:void receive_data_from_displayboard(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_from_displayboard(uint8_t *pdata)
{
  
   if(pdata[1] == 0x02){ //the second display board DEVICE_ID 

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[3] == 0x01){ //open
          
          buzzer_sound();
		  g_disp.g_second_disp_flag = 1;
		  g_pro.gpower_on = power_on;
          SendWifiData_Answer_Cmd(CMD_POWER,0x01); //WT.EDIT 2025.01.07 
          osDelay(5);
        }
        else{ //close 
         
          buzzer_sound();
		  g_disp.g_second_disp_flag = 1;
		  g_pro.gpower_on = power_off;
          power_off_test_counter++;
		 SendWifiData_Answer_Cmd(CMD_POWER,0x0); //WT.EDIT 2025.01.07
		 osDelay(5);

        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){
	 	if(g_pro.gpower_on == power_on){
		 
          buzzer_sound();
          g_pro.gDry = 1;
		   LED_DRY_ON();
		  //manual close flag :
		  // SendWifiData_Answer_Cmd(CMD_PTC,0x01); //WT.EDIT 2025.01.07
		  g_pro.g_manual_shutoff_dry_flag = 0;
		  if(g_pro.works_two_hours_interval_flag==0){
		      DRY_OPEN();
		  }

	 	 
         if(g_wifi.gwifi_link_net_state_flag==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(50);//HAL_Delay(350);
          }
       
       }
       }
       else if(pdata[3] == 0x0){
	   	 if(g_pro.gpower_on == power_on){
		  g_pro.g_manual_shutoff_dry_flag = 1;
          buzzer_sound();
          g_pro.gDry =0;
		  LED_DRY_OFF();
          DRY_CLOSE();
		  if(g_disp.g_second_disp_flag ==1){
		  //SendWifiData_Answer_Cmd(CMD_PTC,0x0); //WT.EDIT 2025.01.07
		  //manual close flag :
		  }
            
         if(g_wifi.gwifi_link_net_state_flag==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(50);//HAL_Delay(350);
          }
	   	 }
       }
    
     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
       if(g_pro.gpower_on == power_on){
	   	  
            buzzer_sound();
			g_pro.gPlasma = 1;
		    LED_PLASMA_ON();
		    if(g_pro.works_two_hours_interval_flag==0){
                PLASMA_OPEN();
		   }
		if(g_wifi.gwifi_link_net_state_flag==1){
           MqttData_Publish_SetPlasma(1);
		       osDelay(50);//HAL_Delay(350);
          }
           
        }
        }
        else if(pdata[3] == 0x0){
        if(g_pro.gpower_on == power_on){ 
            buzzer_sound();
			g_pro.gPlasma = 0;
		    LED_PLASMA_OFF();
		     PLASMA_CLOSE();
		   
		    if(g_wifi.gwifi_link_net_state_flag==1){
              MqttData_Publish_SetPlasma(0);
		       osDelay(50);//HAL_Delay(350);
            }
           
        }
        }   
      break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open 
          if(g_pro.gpower_on == power_on){ 
            buzzer_sound();
			g_pro.gMouse = 1;
		    LED_MOUSE_ON();
		    if(g_pro.works_two_hours_interval_flag==0){
                mouse_open();
		   }
		if(g_wifi.gwifi_link_net_state_flag==1){
           MqttData_Publish_SetUltrasonic(1);
		       osDelay(50);//HAL_Delay(350);
          }
           
          }
       }
        else if(pdata[3] == 0x0){ //close 
		 if(g_pro.gpower_on == power_on){ 
					buzzer_sound();
					g_pro.gMouse = 0;
					LED_MOUSE_OFF();
					mouse_close();
          if(g_wifi.gwifi_link_net_state_flag==1){
				MqttData_Publish_SetUltrasonic(0);
			    osDelay(50);//HAL_Delay(350);
			}

        }
       }
    

     break;

      case 0x05: // link wifi command

       if(pdata[3] == 0x01){  // link wifi 
        if(g_pro.gpower_on == power_on){ 
		  SendWifiData_Answer_Cmd(0x05,0x01); //WT.EDIT 2024.12.28
          buzzer_sound();
		  
        
	      g_wifi.gTimer_wifi_led_fast_blink = 0; //look for wifi information 120s,timer.
	      g_wifi.gwifi_link_net_state_flag=0 ; //clear wifi link net flag .repeat be detected wifi state.
		  g_wifi.wifi_led_fast_blink_flag=1;   // led blink flag .
        
         }
        
       	}

     break;

     case 0x06: //buzzer sound command 

        if(pdata[3] == 0x01){  //buzzer sound 

          buzzer_sound();
        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .
        
        }


     break;


	 case 0x10: //has two display board.

	   if(pdata[3] == 0x01){ 
           g_disp.g_second_disp_flag = 1; 

	    }
		else{
		   g_disp.g_second_disp_flag = 0; 


		}
     break;

	 case 0x11: //the second display board set temperature value 

			
			if(pdata[4] == 0x01){  //no buzzer sound, 
             
             if(g_pro.gpower_on == power_on){ 
				//buzzer_sound();
               g_pro.g_manual_shutoff_dry_flag =0;
                g_pro.key_set_temperature_flag=1;
				
				      
                
			    g_pro.gTimer_input_set_temp_timer= 0;
			   

  				g_pro.gset_temperture_value = pdata[5];
				g_wifi.wifi_set_temperature_value = pdata[5];
				g_pro.gTimer_switch_temp_hum = 0;

				TM1639_Display_Temperature(pdata[5]);
				 if(g_wifi.gwifi_link_net_state_flag==1){
			       MqttData_Publis_SetTemp(g_wifi.wifi_set_temperature_value);
		           osDelay(50);//HAL_Delay(350);
				 }
             }
        }
	     


	 break;

     case 0x16 : //buzzer sound command with answer .

      
       if(pdata[3] == 0x01){  //buzzer sound 

          if(g_pro.gpower_on == power_on){  
          SendWifiData_Answer_Cmd(0x16,0x01); //WT.EDIT 2025.01.07
          buzzer_sound();

          }
            
       }
        else if(pdata[3] == 0x0){ // don't buzzer sound .
 
 
 
        }


     break;


      case 0x1A: //read 温度数据

       if(pdata[3] == 0x0F){ //数据
       
	     

          

	   }
      break;

      case 0x1B: //湿度数据

        if(pdata[3] == 0x0F){ //数据
            

        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[3] == 0x0F){ //数据



        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;

	  
	 case 0x2A: //
	 
			if(pdata[3] == 0x0F){ //数据
	         
			    if(g_pro.gpower_on == power_on){ 
				buzzer_sound();
               g_pro.g_manual_shutoff_dry_flag =0;
                g_pro.key_set_temperature_flag=1;
				
				      
                
			    g_pro.gTimer_input_set_temp_timer= 0;
			   

  				g_pro.gset_temperture_value = pdata[4];
				g_wifi.wifi_set_temperature_value = pdata[4];
				g_pro.gTimer_switch_temp_hum = 0;
                g_disp.g_set_temp_value_flag = 1;
				
//				DHT11_Display_Data(0);
//				 if(g_wifi.gwifi_link_net_state_flag==1){
//			       MqttData_Publis_SetTemp(g_wifi.wifi_set_temperature_value);
//		           osDelay(50);//HAL_Delay(350);
//				 }
             }
				
	 
			}
	 break;

	  

     case 0x22: //PTC打开关闭指令,buzzer don't sound,温度对比后的指令

      if(pdata[3] == 0x01){
        
        if(g_pro.gpower_on == power_on){

		g_pro.g_manual_shutoff_dry_flag=0;
        g_pro.gDry = 1;
		LED_DRY_ON();
     	if(g_pro.works_two_hours_interval_flag==0){
		  	DRY_OPEN();
     	 }

	 	 
         if(g_wifi.gwifi_link_net_state_flag==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(20);//HAL_Delay(350);
          }
       
       
				}
			}
      else if(pdata[3] == 0x0){
        if(g_pro.gpower_on == power_on){

		   g_pro.g_manual_shutoff_dry_flag=0;
         
            g_pro.gDry =0;
		    LED_DRY_OFF();
          	DRY_CLOSE();
		  
		
            
         if(g_wifi.gwifi_link_net_state_flag==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(20);//HAL_Delay(350);
          }
	   	 
       
      }
		}
     break;

     case 0x27: //smart phone set AI mode

      if(pdata[3] == 0x02){ //don't AI
       
        
          
       }
       else if(pdata[3] == 0x01){ //AI mode 
       
      
       }


     break;

     case 0xFF: //copy send cmd acknowlege

	 copy_receive_data(pdata[3],pdata[4]);

	       
     break;

	 default:
	 break;
        
     
     }

   }
 
}

/********************************************************************
	*
	*Function Name: static void copy_receive_data(uint8_t cmd,uint8_t data)
	*Function: mainboard of key be pressed that the second display board receive
	*          command and repeat mainboard order and  send to mainboard,
	*		   then mainboard run command .
	*Input Ref:NO
	*R
	*
*********************************************************************/
static void copy_receive_data(uint8_t cmd,uint8_t data)
{
     switch(cmd){

       case CMD_POWER:

	        if(data == 1){
                buzzer_sound();
			    g_pro.gpower_on = power_on;

			}
			else{
               buzzer_sound();
			   g_pro.gpower_on = power_off;


			}
	 

	   break;
	  

	   case CMD_PTC :
	   	if(data == 1){

		   buzzer_sound();
		   g_pro.gDry=1;
		   LED_DRY_ON();
		   if(g_pro.works_two_hours_interval_flag ==0){

               DRY_OPEN();
              }

		  }
		  else{
			  g_pro.gDry=0;

              DRY_CLOSE();
              LED_DRY_OFF();

		 }

	   break;

	   case CMD_CONNECT_WIFI:
	   	if(data == 1){
			buzzer_sound();
            g_key.key_long_power_flag =  KEY_LONG_POWER; //wifi led blink fast .
			g_wifi.gTimer_wifi_led_fast_blink = 0; //time start 120s ->look for wifi information 120s,timer.
			g_wifi.gwifi_link_net_state_flag=0 ; //clear wifi link net flag .repeat be detected wifi state.
			g_wifi.wifi_led_fast_blink_flag=1;   // led blink flag .

		}
		else{



		}

	   break;


	 }


}


