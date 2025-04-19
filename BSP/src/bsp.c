/*
 * bsp.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"


process_t g_pro;

 uint16_t mainboard_time;

 static void mainboard_special_fun(void);

/******************************************************************************
	*
	*Function Name:void bsp_init(void)
	*Funcion: 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void bsp_init(void)
{
 delay_init(64); 
 dht11_init();
 key_referen_init();


}

/******************************************************************************
	*
	*Function Name:void mainboard_fun_handler(void)
	*Funcion: dry,plasma,mouse,fan ADC
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void mainboard_fun_handler(void)
{
   //static uint16_t mainboard_time;

   mainboard_time ++ ;

   if(mainboard_time > 100){// 2s  //300 ~= 6s, 50 ~=1s
      mainboard_time=0;


   if(g_pro.gAI == 1){

       LED_AI_ON();
   	
   	}
    else{
       LED_AI_OFF();

	}

	if(g_pro.gDry == 1 && read_wifi_dry_value()==0){
		DRY_OPEN();
		LED_DRY_ON();
	}
	else{
		LED_DRY_OFF();
		DRY_CLOSE();

	}

	if(g_pro.gMouse == 1){
		LED_MOUSE_ON();
		mouse_open();
	}
	else{
		LED_MOUSE_OFF();
		mouse_close();
	}

	if(g_pro.gPlasma == 1){
		LED_PLASMA_ON();
		PLASMA_OPEN();
	}
	else{
		LED_PLASMA_OFF();
		PLASMA_CLOSE();
	}

	Fan_RunSpeed_Fun();
	
   	}
	

}

static void mainboard_special_fun(void)
{
   //static uint16_t mainboard_time;

   if(g_pro.gAI == 1){

       LED_AI_ON();
	   LED_AI_ON();
   	
   	}
    else{
       LED_AI_OFF();
	   LED_AI_OFF();

	}

	if(g_pro.gDry == 1){
		DRY_OPEN();
		LED_DRY_ON();
	}
	else{
		DRY_CLOSE();
		LED_DRY_OFF();

	}

	if(g_pro.gMouse == 1){
		mouse_open();
		LED_MOUSE_ON();
	}
	else{
		mouse_close();
		LED_MOUSE_OFF();
	}

	if(g_pro.gPlasma == 1){
		PLASMA_OPEN();
		LED_PLASMA_ON();
	}
	else{
		PLASMA_CLOSE();
		LED_PLASMA_OFF();
	}
	
   Fan_RunSpeed_Fun();
	

}

void mainboard_close_all_fun(void)
{
	DRY_CLOSE();
	mouse_close();

	PLASMA_CLOSE();


}


/**********************************************************************
    *
    *Functin Name: void works_run_two_hours_state(void)
    *Function :  
    *Input Ref: NO
    *Return Ref: NO
    *
************************************************************************/
void works_run_two_hours_state(void)
{
   static uint8_t timer_fan_flag;

   #if TEST_UNIT
	if(g_pro.gTimer_two_hours_counter > 300 && g_pro.works_two_hours_interval_flag==0){ //five minutes 5x60=300s


   #else 
    if(g_pro.gTimer_two_hours_counter > 7200 ){ //two hours

   #endif 
	g_pro.gTimer_two_hours_counter= 0;

    g_pro.works_two_hours_interval_flag=1;

	PLASMA_CLOSE(); //
	DRY_CLOSE();
	mouse_close();
    g_pro.gTimer_fan_run_one_minute=0;


	timer_fan_flag=1;

   
   }

   switch(g_pro.works_two_hours_interval_flag){

    case 1:

   

     #if TEST_UNIT 
	 if(g_pro.gTimer_two_hours_counter  > 600){ //2minutes x 60s = 120s
           
         g_pro.gTimer_two_hours_counter =0;  
		 
         g_pro.works_two_hours_interval_flag=0;
    
     

         mainboard_special_fun();
            
      }
     #else 

      if(g_pro.gTimer_two_hours_counter  > 600){ //10*60s=600s
         g_pro.gTimer_two_hours_counter =0;  
		 
         g_pro.works_two_hours_interval_flag=0;
    
     

         mainboard_special_fun();
                
      }


      #endif 

	 if(timer_fan_flag ==1){

	      if(g_pro.gTimer_fan_run_one_minute < 60){
	  
	              Fan_Full_Speed();
				  PLASMA_CLOSE(); //
				  DRY_CLOSE();
	              mouse_close();
	        } 
			else if(g_pro.gTimer_fan_run_one_minute > 59){
	           
			   g_pro.gTimer_fan_run_one_minute=0;
			
			  timer_fan_flag=0;
             
			   FAN_Stop();
	       }

	  }

   
    break;

    case 0:

	    mainboard_fun_handler();
        
    break;
   }

}




