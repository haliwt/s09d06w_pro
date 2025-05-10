/*
 * bsp_power.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_POWER_H_
#define BSP_INC_BSP_POWER_H_
#include "main.h"

typedef enum{
  close,
  open,

}disp_powert_state;


void power_onoff_handler(uint8_t data);


void power_on_run_handler(void);

void power_off_run_handler(void);


void power_on_init_ref(void);



#endif /* BSP_INC_BSP_POWER_H_ */

