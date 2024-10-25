#ifndef __MQTT_H
#define __MQTT_H 			   
#include "stm32f4xx.h"



#define User_ESP8266_SSID                     "testa"     //wifi���ȵ���
#define User_ESP8266_PWD                      "81306615"      //wifi���ȵ�����
#define User_NULL                             "NULL" 
#define User_ESP8266_client_id                "hgaqAjDLgfW.mqtt_fx|securemode=2\,signmethod=hmacsha256\,timestamp=1657612067672|"   //MQTTclientID ���ڱ�־client���  �256�ֽ�
#define User_ESP8266_username                 "mqtt_fx&hgaqAjDLgfW"						//���ڵ�¼ MQTT ������ �� username, � 64 �ֽ�	
#define User_ESP8266_password			      "7fdaf24132f513971c81395d6c7e3eb627a07450214450de1e2c508a496c2f67"          //���ڵ�¼ MQTT ������ �� password, � 64 �ֽ�
#define User_ESP8266_MQTTServer_IP            "hgaqAjDLgfW.iot-06z00eymyrvtr8r.mqtt.iothub.aliyuncs.com"       //MQTT������
#define User_ESP8266_MQTTServer_PORT           1883                                                  //�������˿ں�
#define User_ESP8266_MQTTServer_Topic         "/sys/hgaqAjDLgfW/mqtt_fx/thing/service/property/set"    //����MQTT����

void ESP8266_STA_MQTTClient_Test(void);
#endif
