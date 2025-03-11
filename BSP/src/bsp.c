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

	if(g_pro.gDry == 1){
		DRY_OPEN();
	}
	else{
		DRY_CLOSE();

	}

	if(g_pro.gMouse == 1){
		mouse_open();
	}
	else{
		mouse_close();
	}

	if(g_pro.gPlasma == 1){
		PLASMA_OPEN();
	}
	else{
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
   	
   	}
    else{
       LED_AI_OFF();

	}

	if(g_pro.gDry == 1){
		DRY_OPEN();
	}
	else{
		DRY_CLOSE();

	}

	if(g_pro.gMouse == 1){
		mouse_open();
	}
	else{
		mouse_close();
	}

	if(g_pro.gPlasma == 1){
		PLASMA_OPEN();
	}
	else{
		PLASMA_CLOSE();
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


   if(g_pro.gTimer_two_hours_counter > 7200 && g_pro.gworks_normal_two_hours==0){ //two hours
	g_pro.gTimer_two_hours_counter= 0;

    g_pro.gworks_normal_two_hours=1;

	PLASMA_CLOSE(); //
	DRY_CLOSE();
	mouse_close();
    g_pro.gTimer_fan_run_one_minute=0;


	timer_fan_flag=1;

   
   }

   switch(g_pro.gworks_normal_two_hours){

    case 1:

   

     #if TEST_UNIT 
	 if(g_pro.gTimer_two_hours_counter  > 2){ //10
           
             check_time=0;
             gctl_t.gTimer_fan_adc_times =0; //ADC be detected must be run 60s,after be detected ADC
		     stopHours_flag=0;
             gpro_t.stopTwoHours_flag=0;
             mainboard_special_fun();
            
      }
     #else 

      if(g_pro.gTimer_two_hours_counter  > 10){ //10
         g_pro.gTimer_two_hours_counter =0;  
		 
         g_pro.gworks_normal_two_hours=0;
    
     

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




