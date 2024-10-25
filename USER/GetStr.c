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
uint8_t  *strx; //��ȡIMEI�Լ��жϽ�������ָ��
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
  * @brief  �ַ�����ȡ����
  * @param  None
  * @retval None
  */
//GetStr(ESP8266_Fram_Record_Struct .Data_RX_BUF);
void GetStr(uint8_t *RxData4)
{
	uint8_t temp=0;
	
	busyp=strstr((const char*)RxData4,(const char*)"busy p...");//�Ƿ����֡ͷ ����ַ�������ȡһ���ؼ���
	{
		if(busyp!=NULL)
		{
			printf("esp restart");
			ESP8266_Rst();
		}
	}
	
	error1=strstr((const char*)RxData4,(const char*)"ERROR");//�Ƿ����֡ͷ ����ַ�������ȡһ���ؼ���
	{
		if(error1!=NULL)
		{
			printf("esp restart");
			ESP8266_Rst();
		}
	}
	
	wtd1=strstr((const char*)RxData4,(const char*)"wtd");//�Ƿ����֡ͷ ����ַ�������ȡһ���ؼ���
	{
		if(wtd1!=NULL)
		{
			printf("esp restart");
			ESP8266_Rst();
			
		}
	}
	
	
	strx=strstr((const char*)RxData4,(const char*)"+MQTTSUBRECV:0");//�Ƿ����֡ͷ ����ַ�������ȡһ���ؼ���
	{
		if(strx!=NULL)
		{
			strx=strstr((const char*)RxData4,(const char*)"\"Te\":");
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyTeData[temp]=strx[5+temp];//MyTeData���ַ���
					  if(MyTeData[temp]==',')
						{
							MyTeData[temp]=0;
							printf("Te:%s\r\n",MyTeData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"At\":");//����OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyAtData[temp]=strx[5+temp];//MyTeData���ַ���
					  if(MyAtData[temp]==',')
						{
							MyAtData[temp]=0;
							printf("At:%s\r\n",MyAtData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"Co\":");//����OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyCoData[temp]=strx[5+temp];//MyTeData���ַ���
					  if(MyCoData[temp]==',')
						{
							MyCoData[temp]=0;
							printf("Co:%s\r\n",MyCoData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"Hu\":");//����OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyHuData[temp]=strx[5+temp];//MyTeData���ַ���
					  if(MyHuData[temp]==',')
						{
							MyHuData[temp]=0;
							printf("Hu:%s\r\n",MyHuData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"\"Li\":");//����OK
			if(strx!=NULL)
			{
				for(temp=0;temp<20;temp++)
				{
						MyLiData[temp]=strx[5+temp];//MyTeData���ַ���
					  if(MyLiData[temp]=='}')
						{
							MyLiData[temp]=0;
							printf("Li:%s\r\n",MyLiData);
							break;
						}
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"pf\":");//����OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("pf״̬0\r\n");
				}
				else if(strx[6]=='1')
				{
					printf("pf״̬1\r\n");
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"tf\":");//����OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("tf״̬0\r\n");
				}
				else if(strx[6]=='1')
				{
					printf("tf״̬1\r\n");
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"af\":");//����OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("�ܿ���״̬0\r\n");
				}
				else if(strx[6]=='1')
				{
					printf("�ܿ���״̬1\r\n");
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"gf\":");//����OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("��״̬0\r\n");  //��
					delay_ms(500);
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);
					delay_ms(500);
				}
				else if(strx[6]=='1')
				{
					printf("��״̬1\r\n");  
					GPIO_SetBits(GPIOD, GPIO_Pin_15);
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"yf\":");//����OK
			if(strx!=NULL)
			{
				if(strx[6]=='0')
				{
					printf("����״̬0\r\n");  //����
					delay_ms(500);
				    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
					delay_ms(500);
//GPIO_SetBits(GPIOD, GPIO_Pin_14);
				}
				else if(strx[6]=='1')
				{
					printf("����״̬1\r\n");
					GPIO_SetBits(GPIOD, GPIO_Pin_14);
					//GPIO_ResetBits(GPIOD, GPIO_Pin_14);
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"zdkz\":");//����OK
			if(strx!=NULL)
			{
				if(strx[8]=='0')
				{
					printf("�Զ�����״̬0\r\n"); //�Զ�����
					g=0;
				}
				else if(strx[8]=='1')
				{
					printf("�Զ�����״̬1\r\n");
					g=1;
					
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"ggf\":");//����OK
			if(strx!=NULL)
			{
				if(strx[7]=='0')
				{
					printf("���״̬0\r\n");  //���
					delay_ms(500);
					GPIO_ResetBits(GPIOD, GPIO_Pin_12);
					delay_ms(500);
				}
				else if(strx[7]=='1')
				{
					printf("���״̬1\r\n");
					GPIO_SetBits(GPIOD, GPIO_Pin_12);
				}
			}
			strx=strstr((const char*)RxData4,(const char*)"{\"f\":");//����OK
			if(strx!=NULL)
			{
				if(strx[5]=='0')
				{
					printf("������״̬0\r\n");  //������
					delay_ms(500);
					GPIO_ResetBits(GPIOD, GPIO_Pin_13);
					delay_ms(500);
				}
				else if(strx[5]=='1')
				{
					printf("������״̬1\r\n");
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
  * @brief  �������ݽ��ս���
  * @param  None
  * @retval None
  */
void ReciveDtaThread()
{
	if(MyReciveSta==1)//���յ�����
	{
		if(MyReciveLenth>50)
		{
			delay_ms(150);
			printf("���յ����ݿ�ʼ����\r\n");
      GetStr(MyBuffer);
			printf("���ݴ������\r\n");
      MyBufferClear();
			printf("��ջ������\r\n");

		}

	}
}
/**
  * @brief  ASIICתHex
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
			{	TIM3_Int_Init(10000,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms 
			}
	}
/*
	if((clock() - start1)==CLOCKS_PER_SEC )
	{
		
		 GPIO_SetBits(GPIOD, GPIO_Pin_13);//����
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
		GPIO_SetBits(GPIOD, GPIO_Pin_14);//����
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
//�ڱ�֤�¶����˵�ǰ���£���ʱͨ�磬�ų�ʪ��;��ˮ�������ܱ����ң�����¶ȡ��ȵ������������ʱ�ټӴ�ͨ�磬�ų�ʪ��
	
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
		 GPIO_SetBits(GPIOD, GPIO_Pin_12);//���
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
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		q++;
		if(q==1800)
    {
			GPIO_ResetBits(GPIOD, GPIO_Pin_13); //����
			q=0;
		  mm=0;
     }
				
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}

int o=0;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		o++;
		if(o==1800)
    {
			GPIO_SetBits(GPIOD, GPIO_Pin_13);//����
		  x=1;  
			n=0;
			o=0;

     }
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

int y=0;

int flag =0;
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
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