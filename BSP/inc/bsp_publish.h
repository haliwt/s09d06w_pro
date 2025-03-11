/*
 * bsp_publish.h
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */

#ifndef __BSP_PUBLISH_H_
#define __BSP_PUBLISH_H_
#include "main.h"

enum pub_Signal {                   /* enumeration for CParser signals */
  tencent_cloud_init,
  tencent_cloud_real_times,
};

typedef enum{

   ptc_temp_warning =0x01,
   fan_warning,
   plasma_warning,
   ultrasonic_warning,


}warning_item;






void Publish_Data_ToTencent_Initial_Data(void);
void Publish_Data_ToTencent_Update_Data(void);

void Publish_Data_Warning(warning_item whichone,uint8_t warning);




#endif /* BSP_INC_BSP_PUBLISH_H_ */
