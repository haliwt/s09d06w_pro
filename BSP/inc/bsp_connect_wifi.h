/*
 * bsp_connect_link.h
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */

#ifndef __BSP_CONNECT_LINK_H_
#define __BSP_CONNECT_LINK_H_
#include "main.h"




void wifi_auto_detected_link_state(void);

void link_wifi_to_tencent_handler(uint8_t data);

void wifi_communication_tnecent_handler(void);

void getBeijingTime_cofirmLinkNetState_handler(void);



#endif /* BSP_INC_BSP_CONNECT_LINK_H_ */
