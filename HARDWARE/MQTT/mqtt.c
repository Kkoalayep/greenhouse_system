#include "mqtt.h"
#include "esp8266.h"


void ESP8266_STA_MQTTClient_Test(void)
{
		printf("正在配置ESP8266参数\r\n");
    ESP8266_AT_Test();
    ESP8266_Net_Mode_Choose(STA_AP);
	  ESP8266_Send_AT_Cmd ( "AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"", "OK", NULL, 2500);
    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
	  ESP8266_MQTTUSERCFG(User_NULL,User_ESP8266_username,User_ESP8266_password);
	  ESP8266_Send_AT_Cmd( "AT+MQTTCLIENTID=0,\"hgaqAjDLgfW.mqtt_fx|securemode=2\\,signmethod=hmacsha256\\,timestamp=1657786257263|\"", "OK", NULL, 500 );
	  ESP8266_Send_AT_Cmd("AT+MQTTCONN=0,\"hgaqAjDLgfW.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1","OK", NULL, 5000 );
	  ESP8266_MQTTSUB( User_ESP8266_MQTTServer_Topic);
	  ESP8266_MQTTSUB( "/sys/hgaqAjDLgfW/mqtt_fx//user/get");
		printf("\r\nMQTT配置完成");
}
