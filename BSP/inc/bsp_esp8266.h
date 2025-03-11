/*
 * bsp_esp8266.h
 *
 *  Created on: 2025年3月10日
 *      Author: Administrator
 */

#ifndef __BSP_ESP8266_H_
#define __BSP_ESP8266_H_
#include "main.h"

#define SSID                    "UYIKIA"
#define PASSWD                  "20329263"


#define TOPIC                  "open"

#define TOPIC_VALUE            1
//#define SMARTCONFIG      			0


#define PRODUCT_ID              "EHQB1P53IH" //production ID 
//#define DEVUICE_NAME            "UYIJIA01-afcc8"      //device Name:
#define DEVICE_SECRET           "5xdQMgx7ZHznYyQK6wvdmolc"//"5xdQMgx7ZHznYyQK6wvdmolc"  //Secret key 
#define PRODUCT_REGION          "ap-guangzhou" //filed name 

//#define WIFI_IC_ENABLE()         HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_SET)
//#define WIFI_IC_DISABLE()        HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_RESET)


typedef enum _esp8266_para{

	wifi_set_restor=0x01,
	wifi_set_cwmode,
	wifi_set_softap,
	wifi_set_tcdevreg,
	wifi_set_tcsap,
	wifi_login_tencent,
	wifi_link_smartphone,
	wifi_inquire_register_codes,


}esp8266_para;

extern uint32_t randomName[1];



void InitWifiModule_Hardware(void);



void InitWifiModule(void);

void Wifi_Link_SmartConfig_Handler(void);

void SmartPhone_LinkTencent_Cloud(void);

uint8_t at_send_data(uint8_t *pdata, uint16_t len);
void Wifi_Rx_InputInfo_Handler(void);

void Wifi_SoftAP_Config_Handler(void);
void SmartPhone_TryToLink_TencentCloud(void);
void wifi_Disconnect_Fun(void);
void Get_BeiJing_Time_Cmd(void);
void Get_Beijing_Time(void);
void PowerOn_Self_Auto_Link_Tencent_Cloud(void);

void Reconnection_Wifi_Order(void);
void AutoRepeate_Link_Tencent_Cloud(void);
void AutoConnect_SmartPhone_TencentCloud(void);
void ReConnect_Wifi_Net_ATReset_Hardware(void);




#endif /* BSP_INC_BSP_ESP8266_H_ */
