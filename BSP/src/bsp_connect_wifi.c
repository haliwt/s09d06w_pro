/*
 * bsp_connect_link.c
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */
#include "bsp.h"




typedef struct{

  uint8_t beijing_step;
   uint8_t auto_link_net_flag;


}wifi_get_beijing_t;


wifi_get_beijing_t gl_msgb;

uint8_t power_on_login_tencent_cloud_flag ;

static void confirm_wifi_link_net_state(void);

static void Auto_InitWifiModule_Hardware(void);

static void Auto_SmartPhone_TryToLink_TencentCloud(void);


/********************************************************************************
	*
	*Functin Name:void wifi_communication_tnecent_handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void wifi_communication_tnecent_handler(void)
{

   if(g_wifi.get_rx_beijing_time_enable==0){
    
         Tencent_Cloud_Rx_Handler();
    	 Json_Parse_Command_Fun();
  
   }
}

/**********************************************************************
	*
	*Functin Name: void wifi_auto_detected_link_state(void)
	*Function :
	*Input Ref:  NO
	*Return Ref: NO
	*
**********************************************************************/
void wifi_auto_detected_link_state(void)
{
    static uint8_t dc_power_on;
	if(power_on_login_tencent_cloud_flag < 5 && g_wifi.gwifi_link_net_state_flag==0){
		
      g_wifi.linking_tencent_cloud_doing = 1;
     

      Auto_InitWifiModule_Hardware();//InitWifiModule();
      Auto_SmartPhone_TryToLink_TencentCloud();

	  
	 }

      if(g_wifi.gwifi_link_net_state_flag==1  && dc_power_on ==0){
              
            dc_power_on++;
           //wifi_t.linking_tencent_cloud_doing = 0;
           g_wifi.linking_tencent_cloud_doing  =0;
           //gpro_t.process_run_step=0;
        
          if(g_pro.gpower_on == power_off){
		     MqttData_Publish_PowerOff_Ref();
             HAL_Delay(200);

          }
		  
          
          Subscriber_Data_FromCloud_Handler();
          HAL_Delay(200);
         

          SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
         
   }

  

   
   
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: power on auto link net fun
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
static void Auto_InitWifiModule_Hardware(void)
{
  
	//WIFI_IC_ENABLE();
	if(power_on_login_tencent_cloud_flag ==0){
	   power_on_login_tencent_cloud_flag=1;
	  // gpro_t.gTimer_power_on_first_link_tencent=0;
	    g_wifi.gTimer_link_net_timer_time =0;
       g_wifi.wifi_rx_data_counter=0;
       g_wifi.linking_tencent_cloud_doing = 1;
	   //at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n")); //
	   at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
       HAL_Delay(1000);

	}
	if(g_wifi.gTimer_link_net_timer_time > 2 &&  power_on_login_tencent_cloud_flag==1 ){
	   g_wifi.gTimer_link_net_timer_time=0;
	   power_on_login_tencent_cloud_flag=2;
	   
	  }
}

static void Auto_SmartPhone_TryToLink_TencentCloud(void)
{
    
  if(power_on_login_tencent_cloud_flag==2 && g_wifi.gTimer_link_net_timer_time >3){
   	power_on_login_tencent_cloud_flag++;
	g_wifi.gTimer_link_net_timer_time=0;
	g_wifi.linking_tencent_cloud_doing  = 1;
    g_wifi.wifi_rx_data_counter=0;

   }
  
   if(power_on_login_tencent_cloud_flag==3 ){
	 power_on_login_tencent_cloud_flag++;
	   g_wifi.gTimer_link_net_timer_time=0;
       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开
       HAL_Delay(1000);
	  
	}
   
   if(g_wifi.gwifi_link_net_state_flag==1 && power_on_login_tencent_cloud_flag ==4){
			//wifi_t.linking_tencent_cloud_doing =0;
			g_wifi.linking_tencent_cloud_doing= 0;
            power_on_login_tencent_cloud_flag++;
            SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
	}
    else if(g_wifi.gwifi_link_net_state_flag ==0 && power_on_login_tencent_cloud_flag ==4){
       power_on_login_tencent_cloud_flag++;
        SendWifiData_To_Cmd(0x1F,0x00);
	    osDelay(5);
    }
}

/********************************************************************************
	*
	*Functin Name:void void getBeijingTime_cofirmLinkNetState_handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void getBeijingTime_cofirmLinkNetState_handler(void)
{
   static uint8_t flag_switch,beijing_step,auto_link_net_flag;
   uint8_t real_hours,real_minutes,real_seconds;
    switch(g_wifi.wifi_get_beijing_step){

     case 0: //WT.EDIT .2024.08.10

      if(g_wifi.gwifi_link_net_state_flag==1 && g_wifi.gTimer_get_data_from_tencent_data > 9){ ////9
       
          g_wifi.gTimer_get_data_from_tencent_data =0;
           flag_switch++;

       

        if(flag_switch > 4 && g_pro.gpower_on == power_on){
             flag_switch=0;
            g_wifi.get_rx_beijing_time_enable=0;
            Subscriber_Data_FromCloud_Handler();
            osDelay(30);//HAL_Delay(200)
             g_wifi.wifi_get_beijing_step = 1; //g_wifi.get_beijing_flag = 1;

            
        }
        else if(g_pro.gpower_on == power_off && flag_switch > 4){ //9*4 =36s.
            flag_switch=0;
            g_wifi.get_rx_beijing_time_enable=0;
            Update_Dht11_Totencent_Value();
            osDelay(30);//HAL_Delay(200) //WT.EDIT 2024.08.10
            
            // gpro_t.get_beijing_flag = 1;
            g_wifi.wifi_get_beijing_step = 1;

        }
        else{ //WT.EDIT 2024.08.10 ADD ITEM
              if(flag_switch > 5){
                 flag_switch=0;
              }
              g_wifi.get_rx_beijing_time_enable=0; 
              //gpro_t.get_beijing_flag = 1;
             

         }
       
    }
    else
     g_wifi.wifi_get_beijing_step = 1;

   break;


   case 1:

        if(g_wifi.gTimer_get_beijing_time > 65 ){//WT.EDIT 2025.01.06 // 65s after repeat auto connect net .

         g_wifi.gTimer_get_beijing_time=0;
        
          if(g_wifi.gwifi_link_net_state_flag==1){

				g_wifi.wifi_get_beijing_step = 2;
           		g_wifi.linking_tencent_cloud_doing  =0; //receive from tencent command state .
                SendWifiData_One_Data(0x1F,0x01); //connect net flag 1: connect .0: don't connect.
                

         }
         else{
              
               SendWifiData_One_Data(0x1F,0x0); //0x1F: 0x1=wifi link net is succes ,0x0 = wifi link net is fail
               g_wifi.wifi_get_beijing_step = 10;
               g_wifi.linking_tencent_cloud_doing  =1; //receive from tencent command state .
             
         }
	
       }
       else{

            g_wifi.wifi_get_beijing_step = 0;
       }

      break;


      case 2: //has bend connected net step.


	    if(g_wifi.get_beijing_time_success == 0){
		

         g_wifi.wifi_get_beijing_step = 3;
         beijing_step =0; //WT.EDIT 2024.08.10
	      
		g_wifi.gTimer_auto_detected_net_state_times =0;
	
		g_wifi.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
	
       }
	   else{

          g_wifi.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		    g_wifi.wifi_get_beijing_step = 3;
			beijing_step =0; //WT.EDIT 2024.08.10

        }


     
    break;


     case 3:

         switch(beijing_step){

         case 0:
            //disable publish data to tencent cloud.
            g_wifi.gTimer_get_data_from_tencent_data=0;

    		
    		Get_BeiJing_Time_Cmd();
    	    osDelay(100);//HAL_Delay(20); //WT.EDIT .2024.08.10//HAL_Delay(20);
    	    beijing_step =1;

         break;

         case 1:
                //disable publish data to tencent cloud.
                g_wifi.gTimer_get_data_from_tencent_data=0;
                g_wifi.get_rx_beijing_time_enable=1;
        		g_wifi.wifi_rx_data_counter =0;
        		Get_Beijing_Time();
              
        	    osDelay(200);//HAL_Delay(20); //WT.EDIT .2024.08.10
                
        	    //g_wifi.get_rx_beijing_time_enable=0; //WT.EDIT .2025.03.11
                beijing_step =2;
          
         

          break; 


          case 2:

            if(g_wifi.wifi_rx_data_array[50] > 0x31 ){
           
                real_hours = (g_wifi.wifi_rx_data_array[41]-0x30)*10 + g_wifi.wifi_rx_data_array[42]-0x30;
                real_minutes =(g_wifi.wifi_rx_data_array[44]-0x30)*10 + g_wifi.wifi_rx_data_array[45]-0x30;
                real_seconds = (g_wifi.wifi_rx_data_array[47]-0x30)*10 + g_wifi.wifi_rx_data_array[48]-0x30;


                   g_wifi.get_rx_beijing_time_enable=0; //enable beijing times
                    g_pro.gdisp_hours_value = real_hours;    
                    g_pro.gdisp_minutes_value = real_minutes;

                    g_pro.gTimer_disp_time_second= real_seconds;
                    g_wifi.get_beijing_time_success = 1;

                    SendWifiData_To_PanelTime(g_pro.gdisp_hours_value,g_pro.gdisp_minutes_value,g_pro.gTimer_disp_time_second);
                    osDelay(50);

                   g_wifi.wifi_get_beijing_step = 6; //WT.EDIT 2025.01.06
                    
                    
                
                }
                else if(g_wifi.wifi_rx_data_array[50] == 0x31){  //"0x31" ASCII = '1'

                   g_wifi.get_rx_beijing_time_enable=0; //enable beijing times
                   if(g_wifi.gwifi_link_net_state_flag==1){
                       
                       g_wifi.gTimer_get_data_from_tencent_data=0;
                       g_wifi.gTimer_get_beijing_time = 50;
                       g_wifi.wifi_get_beijing_step = 6;//WT.EDIT 2025.01.06
                      

                    }
                    else 
                       g_wifi.wifi_get_beijing_step = 6;//WT.EDIT 2025.01.06
            }
            else{

              g_wifi.wifi_get_beijing_step = 6;//WT.EDIT 2025.01.06
              

            }
            break;
         }
    break;


    case 6:

  
       confirm_wifi_link_net_state();
    

       g_wifi.gTimer_auto_detected_net_state_times=0;  

       g_wifi.wifi_get_beijing_step = 0;
    
    break;

    //auto link net 

    case 10:

     if(g_wifi.gTimer_auto_detected_net_state_times > 100){

		    g_wifi.gTimer_auto_detected_net_state_times=0;

    

         if(g_wifi.gwifi_link_net_state_flag==0){ //don't connect wifi net.
            g_wifi.wifi_get_beijing_step = 11;
            g_wifi.linking_tencent_cloud_doing  =1; //receive from tencent command state .
            g_wifi.wifi_rx_data_counter=0;
           
      
           
          }
          else{ //has been connected wifi net 
         
             g_wifi.linking_tencent_cloud_doing  =0; //receive from tencent command state .
             g_wifi.wifi_get_beijing_step = 0;

          }
        
       }

     
     if(g_wifi.gwifi_link_net_state_flag==1){
          g_wifi.wifi_get_beijing_step = 0;

       }
       
     break;


     case 11: //don't connected wifi net .
         if(g_wifi.gwifi_link_net_state_flag==0 && g_wifi.wifi_led_fast_blink_flag==0){

           g_wifi.linking_tencent_cloud_doing =1;
        

          //  WIFI_IC_ENABLE();
       
    		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
            HAL_Delay(1000);
            g_wifi.gTimer_auto_link_net_time =0;
            auto_link_net_flag=1;

           g_wifi.wifi_get_beijing_step = 12;

         }
         else {

             g_wifi.wifi_get_beijing_step = 10;


         }
        
               

     break;


     case 12:

        if(g_wifi.gTimer_auto_link_net_time > 2 && auto_link_net_flag==1){


            g_wifi.gTimer_auto_link_net_time=0;
            g_wifi.wifi_rx_data_counter=0;
	      
             auto_link_net_flag =2;
   
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
            HAL_Delay(1000);
         
           
		
	    }

        if(g_wifi.gTimer_auto_link_net_time > 4 && auto_link_net_flag==2){

            auto_link_net_flag=0 ;
            g_wifi.wifi_get_beijing_step = 13;
           
        }


     break;


     case 13:
       if(g_wifi.gwifi_link_net_state_flag==1){
       
         

        g_wifi.linking_tencent_cloud_doing  =0; //receive from tencent command state .
	    g_wifi.wifi_rx_data_counter=0; //clear USART2 counter is zero

	

          if(g_pro.gpower_on == power_on){
                MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
                osDelay(200);//HAL_Delay(200);

            }
            else if(g_pro.gpower_on == power_off){

               MqttData_Publish_PowerOff_Ref();
               osDelay(200);//HAL_Delay(200);


            }
            Subscriber_Data_FromCloud_Handler();
            osDelay(200);//HAL_Delay(200);

            SendWifiData_One_Data(0x1F,0x01); //0x1F: wifi link net is succes 

             g_wifi.wifi_get_beijing_step = 0;
		
         }
         else{

             g_wifi.wifi_get_beijing_step = 10;
         }

       break;
	

	
        }

}

static void confirm_wifi_link_net_state(void)
{

   HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 5000);


}


