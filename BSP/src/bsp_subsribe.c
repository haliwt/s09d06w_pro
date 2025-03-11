/*
 * bsp_subsribe.c
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */
#include "bsp.h"

void smartphone_timer_power_handler(void)
{

 switch(g_wifi.app_timer_power_on_flag){
		case 0:
          power_on_init_ref();
       	
		  MqttData_Publish_SetOpen(1);  
		  HAL_Delay(200);
		  Update_DHT11_Value();
		  HAL_Delay(200);
	        
		
		   MqttData_Publish_Update_Data();
		   HAL_Delay(200);

         
  
	break;

	case 1: //app timer timing power of 
	       g_pro.gAI =1;

          Parse_Json_phone_timer_power_on_ref();
		  
           if(g_pro.gPlasma==1){//if( gctl_t.gPlasma==1){ //Anion
			

				SendWifiData_To_Cmd(0x03,0x01);
                osDelay(100);
			
			}
			else{
				g_pro.gPlasma=0;//gctl_t.gPlasma =0;
				SendWifiData_To_Cmd(0x03,0x0);
				 osDelay(100);
			}


			if(g_pro.gMouse==1){//if(gctl_t.gUlransonic==1){

					SendWifiData_To_Cmd(0x04,0x01);
					 osDelay(100);
			}
			else {
					g_pro.gMouse=0;//gctl_t.gUlransonic=0;
					SendWifiData_To_Cmd(0x04,0x0);
					 osDelay(100);
			}



			if(g_pro.gDry==1){//if(gctl_t.gDry==1 ||g_dry_open_flag ==1){

				SendWifiData_To_Cmd(0x02,0x01);
				 osDelay(100);
			}
			else{
					g_pro.gDry=0;//gctl_t.gDry=0;
                  
					SendWifiData_To_Cmd(0x02,0x0);
					 osDelay(100);

			}

		     g_wifi.set_wind_speed_value =100;
	
		     property_report_phone_timer_on_data();// MqttData_Publish_Update_Data();
		     HAL_Delay(200);

			
	     break;
		}
			
}

void SetPowerOff_ForDoing(void)
{
   
     g_wifi.set_wind_speed_value =10;
 
   
    g_pro.gDry=0;//gctl_t.gDry = 0;
  
	g_pro.gPlasma=0;//gctl_t.gPlasma =0;       //"杀菌"
	g_pro.gMouse=0;//gctl_t.gUlransonic = 0; // "驱虫"
	g_pro.gAI =1;


    
	PLASMA_CLOSE();//PLASMA_SetLow(); //
	
	DRY_CLOSE();//PTC_SetLow();
	mouse_close();





}



