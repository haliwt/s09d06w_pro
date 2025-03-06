/*
 * bsp_message.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"


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
    *Function Name:void receive_data_fromm_display(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_fromm_display(uint8_t *pdata)
{
  #if 0
   if(pdata[1] == 0x01){

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[3] == 0x01){ //open
          
           
       
            #if 0
            gpro_t.gpower_on = power_on;
            gctl_t.gModel=1;
    	    gctl_t.gFan = 1;
    		dry_open_flag=1;//gctl_t.gDry = 1;
         
    		//gctl_t.gPlasma =1;       //"杀菌"
    		plasma_open_flag =1;
    		ultrasonic_open_flag=1;//gctl_t.gUlransonic = 1; // "驱虫"
    	    gctl_t.gTimer_fan_run_one_minute=0;
            gpro_t.process_run_step=0;     //WT.EDIT 2025.01.11
            fan_run_fun();//SetLevel_Fan_PWMA(10); //WT.EDIT 2024.12.24
            PTC_SetHigh(); // the moment open ptc  //WT.EDIT 2025.01.11
            SendWifiData_Answer_Cmd(0x01,0x01);
            buzzer_sound();//buzzer_sound_fun();

            #endif 
           
            

        }
        else if(pdata[3] == 0x0){ //close 
           
           do{
		   	#if 0
              power_off_action_fun();
              powerOffTunrOff_flag=1;
              gpro_t.gpower_on = power_off;
              SendWifiData_Answer_Cmd(0x01,0x02); //power off .
              buzzer_sound();
			  #endif 
              

           }while(0);


        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){
          buzzer_sound();
         dry_open_flag=1;//gctl_t.gDry = 1;
       

      if(gpro_t.stopTwoHours_flag==0){
           PTC_SetHigh();
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
       
       }
       }
       else if(pdata[3] == 0x0){
          buzzer_sound();
          dry_open_flag=0;//gctl_t.gDry =0;
        
         PTC_SetLow();
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
          }

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
           
            buzzer_sound();
           
           //gctl_t.gPlasma = 1;
           plasma_open_flag=1;
          
           PLASMA_SetHigh();
        }
        else if(pdata[3] == 0x0){
           buzzer_sound();
           
         // gctl_t.gPlasma = 0;
           plasma_open_flag=0;
        
          PLASMA_SetLow();

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open 
          
           ultrasonic_open_flag=1;//gctl_t.gUlransonic =1;

        }
        else if(pdata[3] == 0x0){ //close 

          ultrasonic_open_flag=0; //gctl_t.gUlransonic = 0;

        }


     break;

      case 0x05: // link wifi command

       if(pdata[3] == 0x01){  // link wifi 
         //  buzzer_sound();
         
          gpro_t.link_net_step =0;
	      net_t.wifi_link_net_success=0;
          gpro_t.wifi_led_fast_blink_flag =1;
          gctl_t.wifi_config_net_lable=wifi_set_restor;
		  wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;//2 
		  
          gctl_t.gTimer_linkTencentCounter=0; //total times is 120s
          SendWifiData_Answer_Cmd(0x05,0x01); //WT.EDIT 2024.12.28

        }
        else if(pdata[3] == 0x0){ //don't link wifi 

        }


     break;

     case 0x06: //buzzer sound command 

        if(pdata[3] == 0x01){  //buzzer sound 

         do{
            buzzer_sound();
             pdata[2] =0xff;
          }while(0);
           

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;

     case 0x16 : //buzzer sound command with answer .

      
       if(pdata[3] == 0x01){  //buzzer sound 

           do{
            SendWifiData_Answer_Cmd(0x16,0x01); //WT.EDIT 2025.01.07
            buzzer_sound();
            
            pdata[2] =0xff;

            }while(0);
           
 
        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .
 
 
 
        }


     break;


      case 0x1A: //温度数据

        if(pdata[3] == 0x0F){ //数据

           gctl_t.set_temperature_value = pdata[5] ;
           if(wifi_link_net_state()==1){
             MqttData_Publis_SetTemp(gctl_t.set_temperature_value);
		     osDelay(20);//HAL_Delay(350);
            }

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

     case 0x22: //PTC打开关闭指令,没有蜂鸣器声音。

      if(pdata[3] == 0x01){
        

         dry_open_flag=1;//gctl_t.gDry = 1;
   
        if(gpro_t.stopTwoHours_flag ==0){
              PTC_SetHigh();
             if(wifi_link_net_state()==1){
                  MqttData_Publish_SetPtc(0x01);
    	  	      osDelay(100);//HAL_Delay(350);
               }
          }
          
      }
      else if(pdata[3] == 0x0){
        
         dry_open_flag=0;//gctl_t.gDry =0;
        
      if(gpro_t.stopTwoHours_flag ==0){
        PTC_SetLow();
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
           }

       }
      }

     break;

     case 0x27:

      if(pdata[3] == 0x02){
       
         gctl_t.gModel=2;
         //SendWifiData_Answer_Cmd(0x27,0x02); //don't AI mode,   WT.EDIT 2025.01.06
          if(wifi_link_net_state()==1){
            MqttData_Publish_SetState(2);
	        osDelay(100);//HAL_Delay(350);
           }
        
          
       }
       else if(pdata[3] == 0x01){ //AI mode 
       
         gctl_t.gModel=1;
        // SendWifiData_Answer_Cmd(0x27,0x01); //AI mode,WT.EDIT 2025.01.06
         if(wifi_link_net_state()==1){
             MqttData_Publish_SetState(1);
    	     osDelay(100);//HAL_Delay(350);
         }
       }


     break;

     case 0xFF: //copy send cmd acknowlege
     //power on or power off 
        if(pdata[3]==0x31){ //smart phone normal :power on
            if(pdata[4]==1){ //power on

                gpro_t.receive_copy_cmd = ack_app_power_on;

            }
            else if(pdata[4]==2){ //smart phone normal :power off
               gpro_t.receive_copy_cmd = ack_app_power_off;
            }

        }
        else if(pdata[3]==0x21){ //smart phone of App timer power on .

            if(pdata[4]==1){ //power on

                gpro_t.receive_copy_cmd = ack_app_timer_power_on;

            }
            else if(pdata[4]==2){ //smart phone normal :power off
               gpro_t.receive_copy_cmd = ack_app_power_off;
            }


        }
        else if(pdata[3] == 0x05){ //link wifi command copy command..

        if(pdata[4]==1){

            gpro_t.receive_copy_cmd = ack_wifi_on;

        }


        }

      break;
        
     
     }

   }
  #endif 
}




