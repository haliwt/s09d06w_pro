/*
 * bsp.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"


process_t g_pro;

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
   static uint16_t mainboard_time;

   mainboard_time ++ ;

   if(mainboard_time > 300){
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
	
   	}
	

}


void mainboard_close_all_fun(void)
{
	DRY_CLOSE();
	mouse_close();

	PLASMA_CLOSE();


}



