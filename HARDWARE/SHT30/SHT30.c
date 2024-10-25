#include "SHT30.h"

float humiture[4];
u8 humiture_buff1[20];

float Temperature;
 float Humidity;
//��ʼ��
void SHT30_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
 	SHT30_SCL=1;
	SHT30_SDA_OUT=1;
}
/*��������ACK*/
void SHT30_IIC_ACK(void)
{
  SHT30_IIC_OUTPUT_MODE_SET();
  SHT30_SCL=0;
  delay_us(2); 
  SHT30_SDA_OUT=0;
  delay_us(2);     
  SHT30_SCL=1;
  delay_us(2);                  
  SHT30_SCL=0;                     
  delay_us(1);    
}
 /*����������ACK*/
void SHT30_IIC_NACK(void)
{
  SHT30_IIC_OUTPUT_MODE_SET();
  SHT30_SCL=0;
  delay_us(2); 
  SHT30_SDA_OUT=1;
  delay_us(2);      
  SHT30_SCL=1;
  delay_us(2);                   
  SHT30_SCL=0;                     
  delay_us(1);    
}
 /*�����ȴ��ӻ���ACK*/
u8 SHT30_IIC_wait_ACK(void)
{
    u8 t = 200;
    SHT30_IIC_OUTPUT_MODE_SET();
    SHT30_SDA_OUT=1;//8λ��������ͷ������ߣ�׼������Ӧ��λ 
    delay_us(1);
    SHT30_SCL=0;
    delay_us(1); 
    SHT30_IIC_INPUT_MODE_SET();
    delay_us(1); 
    while(SHT30_SDA_IN)//�ȴ�SHT30Ӧ��
    {
	t--;
	delay_us(1); 
	if(t==0)
	{
	  SHT30_SCL=0;
	  return 1;
	}
	delay_us(1); 
    }
    delay_us(1);      
    SHT30_SCL=1;
    delay_us(1);
    SHT30_SCL=0;             
    delay_us(1);    
    return 0;	
}
/*******************************************************************
����:����I2C����,������I2C��ʼ����.  
********************************************************************/
void SHT30_IIC_Start(void)
{
  SHT30_IIC_OUTPUT_MODE_SET();
  SHT30_SDA_OUT=1;
  SHT30_SCL=1;
  delay_us(4);	
  SHT30_SDA_OUT=0;
  delay_us(4); 
  SHT30_SCL=0;
}
 
/*******************************************************************
����:����I2C����,������I2C��������.  
********************************************************************/
void SHT30_IIC_Stop(void)
{
	SHT30_IIC_OUTPUT_MODE_SET();
	SHT30_SCL=0;
	SHT30_SDA_OUT=0;  
	delay_us(4);	
	SHT30_SCL=1;
	delay_us(4);
	SHT30_SDA_OUT=1;
	delay_us(4);
}
 
/*******************************************************************
�ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(UCHAR c);
����:������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������
********************************************************************/
void  SHT30_IIC_SendByte(u8 byte)
{
	u8  BitCnt;
	SHT30_IIC_OUTPUT_MODE_SET();
	SHT30_SCL=0;
	for(BitCnt=0;BitCnt<8;BitCnt++)//Ҫ���͵����ݳ���Ϊ8λ
	{
		if(byte&0x80) SHT30_SDA_OUT=1;//�жϷ���λ
		else SHT30_SDA_OUT=0; 
		byte<<=1;
		delay_us(2); 
		SHT30_SCL=1;
		delay_us(2);
		SHT30_SCL=0;
		delay_us(2);
	}
}
/*******************************************************************
 �ֽ����ݽ��պ���               
����ԭ��: UCHAR  RcvByte();
����: �������մ���������������  
********************************************************************/    
u8 SHT30_IIC_RcvByte(void)
{
  u8 retc;
  u8 BitCnt;
  retc=0; 
  SHT30_IIC_INPUT_MODE_SET();//��������Ϊ���뷽ʽ
  delay_us(1);                    
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {  
	SHT30_SCL=0;//��ʱ����Ϊ�ͣ�׼����������λ
	delay_us(2);               
	SHT30_SCL=1;//��ʱ����Ϊ��ʹ��������������Ч                
	retc=retc<<1;
	if(SHT30_SDA_IN) retc |=1;//������λ,���յ�����λ����retc�� 
	delay_us(1);
  }
  SHT30_SCL=0;    
  return(retc);
}
/*******************************************************************
 ��ʪ�Ȼ�ȡ����               
����ԭ��: SHT30_read_result(u8 addr);
����: �������մ������ɼ����ϳ���ʪ��
********************************************************************/ 
void SHT30_read_result(u8 addr)
{
	u16 tem,hum;
	u16 buff[6];

	SHT30_IIC_Start();
	SHT30_IIC_SendByte(addr<<1 | write);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
	SHT30_IIC_wait_ACK();
	SHT30_IIC_SendByte(0x2C);
	SHT30_IIC_wait_ACK();
	SHT30_IIC_SendByte(0x06);
	SHT30_IIC_wait_ACK();
	SHT30_IIC_Stop();
	delay_ms(50);
	SHT30_IIC_Start();
	SHT30_IIC_SendByte(addr<<1 | read);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
	if(SHT30_IIC_wait_ACK()==0)
	{
		buff[0]=SHT30_IIC_RcvByte();
		SHT30_IIC_ACK();
		buff[1]=SHT30_IIC_RcvByte();
		SHT30_IIC_ACK();
		buff[2]=SHT30_IIC_RcvByte();
		SHT30_IIC_ACK();
		buff[3]=SHT30_IIC_RcvByte();
		SHT30_IIC_ACK();
		buff[4]=SHT30_IIC_RcvByte();
		SHT30_IIC_ACK();
		buff[5]=SHT30_IIC_RcvByte();
		SHT30_IIC_NACK();
		SHT30_IIC_Stop();
	}
	
	tem = ((buff[0]<<8) | buff[1]);//�¶�ƴ��
	hum = ((buff[3]<<8) | buff[4]);//ʪ��ƴ��
	
	/*ת��ʵ���¶�*/
	Temperature= (175.0f*(float)tem/65535.0f-45.0f) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity= (100.0f*(float)hum/65535.0f);// RH = hum*100 / (2^16-1)
	
//	if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//���˴�������
//	{
////		humiture[0]=Temperature;
////		humiture[2]=Humidity;
//		sprintf(humiture_buff1,"%0.1f*C %0.1f%%",Temperature,Humidity);//111.01*C 100.01%������2λС����
//	}
//	printf("��ʪ�ȣ�%s\n",humiture_buff1);
//	hum=0;
//	tem=0;
			
	//printf("Temperature=%0.1f*C Humidity=%0.1f%%\r\n",Temperature,Humidity);
}



