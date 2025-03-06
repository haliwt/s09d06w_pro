/*
 * bsp_message.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_MESSAGE_H_
#define BSP_INC_BSP_MESSAGE_H_
#include "main.h"

uint8_t bcc_check(const unsigned char *data, int len) ;

void receive_data_fromm_display(uint8_t *pdata);



#endif /* BSP_INC_BSP_MESSAGE_H_ */
