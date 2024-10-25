#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include "esp8266.h" 
#include "time.h"
#include "timer.h"
#include "BH1750.h"
#include "SHT30.h"
#include "sgp30.h"
//extern struct ESP8266_Fram_Record_Struct;
uint8_t CharToHex(uint8_t data);
void ReciveDtaThread();
uint8_t  *strx; //截取IMEI以及判断接收数据指针
uint8_t  *busyp;
uint8_t  *error1;
uint8_t  *wtd1;
uint8_t MyTeData[30]={0};
uint8_t MyAtData[30]={0};
uint8_t MyCoData[30]={0};
uint8_t MyHuData[30]={0};
uint8_t MyLiData[30]={0};
uint8_t MyPyData[30]={0};

extern uint8_t MyBuffer[2048];
extern uint8_t MyReciveSta;
extern uint16_t MyReciveLenth;

u8 g;
/**
  * @brief  字符串截取函数
  * @param  None
  * @retval None
  */
//GetStr(ESP8266_Fram_Record_Struct .Data_RX_BUF);
void GetStr(uint8_t *RxData4)
{
	uint8_t temp=0;
	
	busyp=strstr((const char*)RxData4,(const char*)"busy p...");//是否包含帧头 这个字符串自行取一定关键字
	{
		if(busyp!=NULL)
		{
			printf("esp restart");
			ESP8266_Rst();
		}
	}
	
	error1=strstr((const char*)RxData4,(const char*)"ERROR");//是否包含帧头 这个字符串自行取一定关键字
	{
		if(error1!=NULL)
		{
			printf("esp restart");
			ESP8266_Rst();
		}
	}
	
	wtd1=strstr((const char*)RxData4,(const char*)"wtd");//是否包含帧头 这个字符串自行取一定关键字
	{
		if(wtd1!=NULL)
		{
			printf("esp restart");
			ESP8266_Rst();
			
		}
	}
	
	
	strx=strstr((const char*)RxData4,(const char*)"+MQTTSUBRECV:0");//是否包含帧头 这个字符串自行取一定关键字
	{
		if(strx!=NULL)
		{
			strx=strstr((const char*)RxData4,(const char*)"\"Te\":");
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyTeData[temp]=strx[5+temp];//MyTeData是字符串
					  if(MyTeData[temp]==',')
						{
							MyTeData[temp]=0;
							printf("Te:%s\r\n",MyTeData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"At\":");//返回OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyAtData[temp]=strx[5+temp];//MyTeData是字符串
					  if(MyAtData[temp]==',')
						{
							MyAtData[temp]=0;
							printf("At:%s\r\n",MyAtData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"Co\":");//返回OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyCoData[temp]=strx[5+temp];//MyTeData是字符串
					  if(MyCoData[temp]==',')
						{
							MyCoData[temp]=0;
							printf("Co:%s\r\n",MyCoData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"Hu\":");//返回OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyHuData[temp]=strx[5+temp];//MyTeData是字符串
					  if(MyHuData[temp]==',')
						{
							MyHuData[temp]=0;
							printf("Hu:%s\r\n",MyHuData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"Li\":");//返回OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyLiData[temp]=strx[5+temp];//MyTeData是字符串
					  if(MyLiData[temp]=='}')
						{
							MyLiData[temp]=0;
							printf("Li:%s\r\n",MyLiData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"pf\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("pf状态0\r\n");
				}
				else if(strx[6]=='1')
				{
					printf("pf状态1\r\n");
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"tf\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("tf状态0\r\n");
				}
				else if(strx[6]=='1')
				{
					printf("tf状态1\r\n");
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"af\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("总开关状态0\r\n");
				}
				else if(strx[6]=='1')
				{
					printf("总开关状态1\r\n");
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"gf\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("灯状态0\r\n");  //灯
					delay_ms(500);
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);
					delay_ms(500);
				}
				else if(strx[6]=='1')
				{
					printf("灯状态1\r\n");  
					GPIO_SetBits(GPIOD, GPIO_Pin_15);
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"yf\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("遮阳状态0\r\n");  //遮阳
					delay_ms(500);
				    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
					delay_ms(500);
//GPIO_SetBits(GPIOD, GPIO_Pin_14);
				}
				else if(strx[6]=='1')
				{
					printf("遮阳状态1\r\n");
					GPIO_SetBits(GPIOD, GPIO_Pin_14);
					//GPIO_ResetBits(GPIOD, GPIO_Pin_14);
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"zdkz\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[8]=='0')
				{
					printf("自动控制状态0\r\n"); //自动控制
					g=0;
				}
				else if(strx[8]=='1')
				{
					printf("自动控制状态1\r\n");
					g=1;
					
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"ggf\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[7]=='0')
				{
					printf("灌溉状态0\r\n");  //灌溉
					delay_ms(500);
					GPIO_ResetBits(GPIOD, GPIO_Pin_12);
					delay_ms(500);
				}
				else if(strx[7]=='1')
				{
					printf("灌溉状态1\r\n");
					GPIO_SetBits(GPIOD, GPIO_Pin_12);
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"f\":");//返回OK
			if(strx!=NULL)
			{
				if(strx[5]=='0')
				{
					printf("排气扇状态0\r\n");  //排气扇
					delay_ms(500);
					GPIO_ResetBits(GPIOD, GPIO_Pin_13);
					delay_ms(500);
				}
				else if(strx[5]=='1')
				{
					printf("排气扇状态1\r\n");
					GPIO_SetBits(GPIOD, GPIO_Pin_13);
				}
			}
		}
	}
}
void MyBufferClear()
{
	for(uint16_t temp1=0;temp1<2048;temp1++)
	{
		MyBuffer[temp1]=0;
	}
	MyReciveLenth=0;
	MyReciveSta=0;
}
/**
  * @brief  串口数据接收进程
  * @param  None
  * @retval None
  */
void ReciveDtaThread()
{
	if(MyReciveSta==1)//接收到数据
	{
		if(MyReciveLenth>50)
		{
			delay_ms(150);
			printf("接收到数据开始处理\r\n");
      GetStr(MyBuffer);
			printf("数据处理完毕\r\n");
      MyBufferClear();
			printf("清空缓存完毕\r\n");

		}

	}
}
/**
  * @brief  ASIIC转Hex
  * @param  ASIIC
  * @retval Hex
  */
uint8_t CharToHex(uint8_t data)
{
	switch(data)
	{
		case 0x30:
		{
			return 0x00;
		}
		break;
				case 0x31:
		{
			return 0x01;
		}
		break;
				case 0x32:
		{
			return 0x02;
		}
		break;
				case 0x33:
		{
			return 0x03;
		}
		break;
				case 0x34:
		{
			return 0x04;
		}
		break;
				case 0x35:
		{
			return 0x05;
		}
		break;
				case 0x36:
		{
			return 0x06;
		}
		break;
				case 0x37:
		{
			return 0x07;
		}
		break;
				case 0x38:
		{
			return 0x08;
		}
		break;
				case 0x39:
		{
			return 0x09;
		}
		break;
				case 0x41:
		{
			return 0x0A;
		}
		break;
				case 0x42:
		{
			return 0x0B;
		}
		break;
				case 0x43:
		{
			return 0x0C;
		}
		break;
				case 0x44:
		{
			return 0x0D;
		}
		break;
				case 0x45:
		{
			return 0x0E;
		}
		break;
				case 0x46:
		{
			return 0x0F;
		}
		break;
				case 0x61:
		{
			return 0x0A;
		}
		break;
				case 0x62:
		{
			return 0x0B;
		}
		break;
				case 0x63:
		{
			return 0x0C;
		}
		break;
				case 0x64:
		{
			return 0x0D;
		}
		break;
				case 0x65:
		{
			return 0x0E;
		}
		break;
				case 0x66:
		{
			return 0x0F;
		}
		break;
		default :{return 0x40;break;}
	}
}

 
float	BMP_Pressure,BMP_Temperature;
int j =0,m=0,n=0,nn=0,mm=0,kk=0;
int i=0;
int x=0  ;
void panduan(void)
{
	
//	clock_t start1;
//	clock_t start2;
//	clock_t start3;
//	clock_t start4;
	if(atol(MyTeData) > 30 )//half hour
	{
		n++;
    if(n==100)
			{	TIM3_Int_Init(10000,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms 
			}
	}
/*
	if((clock() - start1)==CLOCKS_PER_SEC )
	{
		
		 GPIO_SetBits(GPIOD, GPIO_Pin_13);//风扇
		 x=1;
		 
	}
	    */
	
	if(x==1&&atol(MyTeData) <26 )
	{
	  nn++;
		if(nn==100)
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		x=0;
		nn=0;
		}
	}	
	
	
	if(atol(MyLiData) > lux)
	{
		    
	  //start2 = clock();
	  j++;
	  
	
	}
	
	if(j==100)
	{
		j=0;
		GPIO_SetBits(GPIOD, GPIO_Pin_14);//遮阳
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		
	}
	
	if(atol(MyLiData) < 120 &&lux >atol(MyLiData))
	{
	  m++;
    if(m==100)
		{
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
    m=0;
		
		}
	
	}
//在保证温度适宜的前提下，及时通风，排出湿气;浇水后立即密闭棚室，提高温度。等到了中午和下午时再加大通风，排出湿气
	
	if(Temperature< 30&&Temperature > 25&& Humidity>70)
	{
	  mm++;
     if(mm==100)
    {
		 GPIO_SetBits(GPIOD, GPIO_Pin_13);
	   TIM2_Int_Init(10000,8400-1);	

     }
	//	 start3 = clock();
	}
	
/*	if((clock() - start3)==CLOCKS3_PER_SEC ) //10m
	{
		
		 GPIO_ResetBits(GPIOD, GPIO_Pin_13); 
	}
	*/
	if(atol(MyHuData)<20)
	{
	 
	 //start4 = clock();
   kk++;
   if(kk==100)
		{
		 GPIO_SetBits(GPIOD, GPIO_Pin_12);//灌溉
		 TIM4_Int_Init(10000,8400-1);	
		}
	
	}

/*	
	if((clock() - start4)==CLOCKS4_PER_SEC )//5m
	{
		
		 GPIO_ResetBits(GPIOD, GPIO_Pin_12); 
	}

*/

}

int q=0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		q++;
		if(q==1800)
    {
			GPIO_ResetBits(GPIOD, GPIO_Pin_13); //风扇
			q=0;
		  mm=0;
     }
				
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

int o=0;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		o++;
		if(o==1800)
    {
			GPIO_SetBits(GPIOD, GPIO_Pin_13);//风扇
		  x=1;  
			n=0;
			o=0;

     }
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

int y=0;

int flag =0;
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		i++;
		if(i==10)
    {
			GPIO_ResetBits(GPIOD, GPIO_Pin_12); 
			i=0;
      kk=0;
       flag=0;
     }
		
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}

	
void panduan1(void)
{
	if (Temperature>27&&flag==0)

	{
    GPIO_SetBits(GPIOD, GPIO_Pin_12);//guangai
		TIM4_Int_Init(10000,8400-1);	
		flag=1;
   }

   if(Humidity>65)
  {
		GPIO_SetBits(GPIOD, GPIO_Pin_13);

  }
  else if(Humidity<64)
{
GPIO_ResetBits(GPIOD, GPIO_Pin_13); 

}
 

}