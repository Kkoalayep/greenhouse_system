#ifndef __MQTT_H
#define __MQTT_H 			   
#include "stm32f4xx.h"



#define User_ESP8266_SSID                     "testa"     //wifi或热点名
#define User_ESP8266_PWD                      "81306615"      //wifi或热点密码
#define User_NULL                             "NULL" 
#define User_ESP8266_client_id                "hgaqAjDLgfW.mqtt_fx|securemode=2\,signmethod=hmacsha256\,timestamp=1657612067672|"   //MQTTclientID 用于标志client身份  最长256字节
#define User_ESP8266_username                 "mqtt_fx&hgaqAjDLgfW"						//用于登录 MQTT 服务器 的 username, 最长 64 字节	
#define User_ESP8266_password			      "7fdaf24132f513971c81395d6c7e3eb627a07450214450de1e2c508a496c2f67"          //用于登录 MQTT 服务器 的 password, 最长 64 字节
#define User_ESP8266_MQTTServer_IP            "hgaqAjDLgfW.iot-06z00eymyrvtr8r.mqtt.iothub.aliyuncs.com"       //MQTT服务器
#define User_ESP8266_MQTTServer_PORT           1883                                                  //服务器端口号
#define User_ESP8266_MQTTServer_Topic         "/sys/hgaqAjDLgfW/mqtt_fx/thing/service/property/set"    //订阅MQTT主题

void ESP8266_STA_MQTTClient_Test(void);
#endif
