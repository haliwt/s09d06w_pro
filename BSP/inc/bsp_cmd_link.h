/*
 * bsp_cmd_link.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef __BSP_CMD_LINK_H_
#define __BSP_CMD_LINK_H_

#include "main.h"

extern uint8_t rx_wifi_data[7];

//extern uint8_t inputBuf[4];

extern uint8_t  inputCmd[30];
extern uint8_t wifiInputBuf[1];


void sendData_Real_TimeHum(uint8_t hum,uint8_t temp);


void SendWifiData_To_PanelWindSpeed(uint8_t dat1);


void SendWifiData_To_PanelTime(uint8_t hours,uint8_t minutes,uint8_t seconds);


//void SendData_Set_Command(uint8_t cmd,uint8_t data);

void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data);


void SendWifiData_To_Data(uint8_t cmd,uint8_t data);
void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data);

void SendData_Set_Command(uint8_t cmd,uint8_t data);


#endif /* BSP_INC_BSP_CMD_LINK_H_ */
