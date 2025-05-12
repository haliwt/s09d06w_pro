#include "bsp.h"



// 定义响应消息类型枚举
typedef enum {
    WIFI_RESP_UNKNOWN,
    WIFI_RESP_CONNECT_SUCCESS,
    WIFI_RESP_CONNECT_FAILED,
    MQTT_RESP_CONNECT_OK,
    MQTT_RESP_CONNECT_FAIL,
    MQTT_RESP_DISCONNECT,
    ERROR_RESP_208,
    TCPRD_RESP_INFO
} WifiResponseType;

// 定义响应模式结构体
typedef struct {
    const char *pattern;
    WifiResponseType type;
} ResponsePattern;

// 预定义的响应模式数组
static const ResponsePattern response_patterns[] = {
    {"+TCSAP:WIFI_CONNECT_SUCCESS", WIFI_RESP_CONNECT_SUCCESS},
    {"+TCSAP:WIFI_CONNECT_FAILED", WIFI_RESP_CONNECT_FAILED},
    {"+TCMQTTCONN:OK", MQTT_RESP_CONNECT_OK},
    {"+TCMQTTCONN:FAIL", MQTT_RESP_CONNECT_FAIL},
    {"+TCMQTTDISCON", MQTT_RESP_DISCONNECT},
    {"+CME ERROR:208", ERROR_RESP_208},
    {"+TCPRDINFOSET:1,\"EHQB1P53IH\"", TCPRD_RESP_INFO},
    {NULL, WIFI_RESP_UNKNOWN} // 结束标记
};

// 识别响应类型函数
static WifiResponseType identify_response(const char *rx_data) {
    if (rx_data == NULL || rx_data[0] == '\0') {
        return WIFI_RESP_UNKNOWN;
    }
    
    for (const ResponsePattern *p = response_patterns; p->pattern != NULL; p++) {
        if (strstr(rx_data, p->pattern) != NULL) {
            return p->type;
        }
    }
    return WIFI_RESP_UNKNOWN;
}

// 处理MQTT连接失败
static void handle_mqtt_failure(void) {
    g_wifi.gwifi_link_net_state_flag = 0;
    g_wifi.wifi_led_fast_blink_flag = 0;
    g_wifi.linking_tencent_cloud_doing = 0;
    g_wifi.get_rx_beijing_time_enable = 0;
}

// 处理MQTT连接成功
static void handle_mqtt_success(void) {
    g_wifi.gwifi_link_net_state_flag = 1;
    g_wifi.linking_tencent_cloud_doing = 0;
    g_wifi.auto_link_cloud_flag = 0xFF;
}



// WiFi响应处理函数
void wifi_parse_handler(void)
{

//   if(g_wifi.wifi_data_parse_flag==1){

//	 g_wifi.wifi_data_parse_flag=0;

     g_wifi.wifi_rx_data_counter = 0;
    g_wifi.once_rx_data_done = 1;
	  
	WifiResponseType resp_type = identify_response((const char*)g_wifi.wifi_rx_data_array);
    
    switch (resp_type) {
        case WIFI_RESP_CONNECT_SUCCESS:
            g_wifi.soft_ap_config_success = 1;
            break;
            
        case WIFI_RESP_CONNECT_FAILED:
            g_wifi.gwifi_link_net_state_flag = 0;
            g_wifi.get_rx_beijing_time_enable = 0;
            g_wifi.linking_tencent_cloud_doing = 1;
            break;
            
        case MQTT_RESP_CONNECT_OK:
            handle_mqtt_success();
            break;
            
        case MQTT_RESP_CONNECT_FAIL:
            handle_mqtt_failure();
            break;
            
        case MQTT_RESP_DISCONNECT:
            g_wifi.gwifi_link_net_state_flag = 0;
            g_wifi.get_rx_beijing_time_enable = 0;
            g_wifi.linking_tencent_cloud_doing = 1;
            break;
            
        case ERROR_RESP_208:
            g_wifi.gwifi_link_net_state_flag = 0;
            g_wifi.linking_tencent_cloud_doing = 0;
            g_wifi.wifi_led_fast_blink_flag = 0;
            break;
            
        case TCPRD_RESP_INFO:
            // 保留配置处理逻辑
            break;
            
        case WIFI_RESP_UNKNOWN:
        default:
            // 可添加未知响应处理或日志记录
            break;
    }

    // 重置接收状态
  
	}
//}


