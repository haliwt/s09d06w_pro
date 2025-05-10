/*
 * bsp_message.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_MESSAGE_H_
#define BSP_INC_BSP_MESSAGE_H_
#include "main.h"


// Command types
#define CMD_POWER           	 0x01
#define CMD_PTC              0x02
#define CMD_PLASMA			 	0x03
#define CMD_MOUSE  				0x04
#define CMD_CONNECT_WIFI         0x05
#define CMD_BUZZER_SOUND         		 0x06
#define CMD_AI 					0x07
#define CMD_PTC_WARNING				0x08
#define CMD_FAN_WARNING				0x09
#define CMD_BUZZER_ACK				0x16
#define DATA_READ_TEMP            0x1A //read temperature value
#define CMD_CONNECT_FLAG         0x1F  //if connect wifi 1-connet wiif is ok .0- don't connect wifi net
#define DATA_SET_TEMP_VALUE       0x2A  //phone APP or mainboard or display board set up temperature value 


uint8_t bcc_check(const unsigned char *data, int len) ;

void receive_data_from_displayboard(uint8_t *pdata);



#endif /* BSP_INC_BSP_MESSAGE_H_ */
