#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "myiic.h"
#include "soil.h"
#include "beep.h"
#include "bmpiic.h"
#include "bmp280.h" 
#include "esp8266.h" 
#include "mqtt.h" 
#include "stdio.h"
#include "timer.h"
#include "BH1750.h"
#include "SHT30.h"
#include "sgp30.h"

//STM32F407¿ª·¢°å ÊµÑé31
//DHT11Êı×ÖÎÂÊª¶È´«¸ĞÆ÷ ÊµÑé  
//ÌÔ±¦µêÆÌ£ºhttp://CHINANUEDC.taobao.com  
void ReciveDtaThread();
extern u32 CO2Data , TVOCData;;//¶¨ÒåCO2Å¨¶È±äÁ¿ÓëTVOCÅ¨¶È±
char CO2Data1[20],TVOCData1[20],BMP_Temperature1[20],BMP_Pressure1[20],temperature1[20],humidity1[20],light1[20],tw1[10],th1[10],fo1[10],fi1[10];
int b=26;
char cCmd [300];

extern u8 g;
extern float Temperature;  	    
extern  float Humidity; 
extern float lux; 
extern	float	BMP_Pressure,BMP_Temperature;

int main(void)
{ 
	int tw,th,fo,fi;
	float	BMP_Pressure_a;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÏµÍ³ÖĞ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init(168);  //³õÊ¼»¯ÑÓÊ±º¯Êı
	uart_init(115200);		//³õÊ¼»¯´®¿Ú²¨ÌØÂÊÎª115200
	delay_ms(100);//µÈ´ıÄ£¿é³õÊ¼»¯Íê³É
	
	ESP8266_Init(115200);
	ESP8266_STA_MQTTClient_Test();
	//BEEP_Init();
	LED_Init();					//³õÊ¼»¯LED 
	Adc_Init();	
	//IIC_Ini();
	BMP_IIC_Init();

	BH1750_Init();			//BH1750³õÊ¼»¯ 
	delay_ms(200);
	SHT30_Init();				//SHT30³õÊ¼»¯
	delay_ms(200);	
	SGP30_Init();   //³õÊ¼»¯SGP30
	delay_ms(100);

	Bmp_Init();
	delay_ms(50);
	/*Configuration Operation*/
	
	printf("\r\nBMP280 ID:0x%d",BMP280_ReadID());	
	delay_ms(50);
	 
	u8 t=0;	
	while(1)
	{   
        LED0=!LED0;//ÌáÊ¾ÏµÍ³ÕıÔÚÔËĞĞ	
		BH1750_Read();
		SHT30_read_result(0x44);
		SGP30_Read_Value();
		delay_ms(500);
		
		
		//while(BMP280_GetStatus(BMP280_MEASURING) != RESET);
		//while(BMP280_GetStatus(BMP280_IM_UPDATE) != RESET);
		//BMP_Temperature = BMP280_Get_Temperature();
		
		BMP_Pressure_a = BMP280_Get_Pressure();
		BMP_Pressure = BMP_Pressure_a * 0.001;
		delay_ms(500);
		/*
		printf("\r\nTemperature %f C Pressure %f Pa",Temperature,BMP_Pressure); 
		
		printf("Temperature=%0.1f*C Humidity=%0.1f%%\r\n",Temperature,Humidity);
		printf("CO2:%dppm TVOC:%dppd\r\n",CO2Data,TVOCData);
		printf("light:%0.1f lux\r\n",lux);
		
		*/
		tw = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12); //ggf    ggz¹à¸È×´Ì¬£¬  
		th = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13);//f       fz ·çÉÈ×´Ì¬
		fo = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14);//yf      yz ÕÚÑô×´Ì¬
		fi = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15);//gf      gz µÆ×´Ì¬
		
		

		
		sprintf(CO2Data1, "%d", CO2Data);sprintf(TVOCData1, "%d", TVOCData);
		sprintf(BMP_Temperature1, "%.5f", Temperature);
		sprintf(BMP_Pressure1, "%.5f", BMP_Pressure);
		//sprintf(temperature1, "%.5f", Temperature);
		sprintf(humidity1, "%.1f", Humidity);sprintf(light1, "%.1f",lux);
		sprintf(tw1, "%d", tw);sprintf(th1, "%d", th);sprintf(fo1, "%d",fo);sprintf(fi1, "%d",fi);
			
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"ggf\\\":%s\\,\\\"f\\\":%s\\,\\\"yf\\\":%s\\,\\\"gf\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",tw1 ,th1 ,fo1 ,fi1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 700);
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"ggz\\\":%s\\,\\\"fz\\\":%s\\,\\\"yz\\\":%s\\,\\\"gz\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",tw1 ,th1 ,fo1 ,fi1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL,700 );
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"Hu\\\":%s\\,\\\"At\\\":%s\\,\\\"Te\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",humidity1,BMP_Pressure1,BMP_Temperature1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 700 );
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"Li\\\":%s\\,\\\"Co\\\":%s\\,\\\"HC\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",light1,CO2Data1,TVOCData1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL,700 );
	

/*		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"ggf\\\":%s\\,\\\"f\\\":%s\\,\\\"yf\\\":%s\\,\\\"gf\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",tw1 ,th1 ,fo1 ,fi1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 1000 );
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"ggz\\\":%s\\,\\\"fz\\\":%s\\,\\\"yz\\\":%s\\,\\\"gz\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",tw1 ,th1 ,fo1 ,fi1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 1000 );
*/

   /*
	sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"ggf\\\":%s\\,\\\"f\\\":%s\\,\\\"yf\\\":%s\\,\\\"gf\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",tw1 ,th1 ,fo1 ,fi1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500);
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"ggz\\\":%s\\,\\\"fz\\\":%s\\,\\\"yz\\\":%s\\,\\\"gz\\\":%s}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",tw1 ,th1 ,fo1 ,fi1 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL,500 );
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"Hu\\\":%d\\,\\\"At\\\":%d\\,\\\"Te\\\":%d}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",123,123,123 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
		sprintf ( cCmd, "AT+MQTTPUB=0,\"/sys/hgaqAjDLgfW/mqtt_fx/thing/event/property/post\",\"{\\\"id\\\":1657529185567\\,\\\"params\\\":{\\\"Li\\\":%d\\,\\\"Co\\\":%d\\,\\\"HC\\\":%d}\\,\\\"version\\\":\\\"1.0.0\\\"\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",1,0",123,123,123 );
    ESP8266_Send_AT_Cmd( cCmd, "OK", NULL,500 );
		*/
		delay_ms(500);
		ReciveDtaThread();
		delay_ms(500);
	//	if(g==1)
	//	{panduan1();}

		
	}	
}

//{"Humidity":30,"temperature":40,"co2":440,"Atmosphere":44,"Env_lux":441}

		/*	if(temperature>b)
		  {  Sound2(100);  }*/


