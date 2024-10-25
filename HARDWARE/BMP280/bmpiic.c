#include "bmpiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC
void BMP_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	BMP_IIC_SCL=1;
	BMP_IIC_SDA=1;
}
//����IIC��ʼ�ź�
void BMP_IIC_Start(void)
{
	BMP_SDA_OUT();     //sda�����
	BMP_IIC_SDA=1;	  	  
	BMP_IIC_SCL=1;
	delay_us(4);
 	BMP_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	BMP_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void BMP_IIC_Stop(void)
{
	BMP_SDA_OUT();//sda�����
	BMP_IIC_SCL=0;
	BMP_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	BMP_IIC_SCL=1; 
	BMP_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 BMP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	BMP_SDA_IN();      //SDA����Ϊ����  
	BMP_IIC_SDA=1;delay_us(1);	   
	BMP_IIC_SCL=1;delay_us(1);	 
	while(BMP_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			BMP_IIC_Stop();
			return 1;
		}
	}
	BMP_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void BMP_IIC_Ack(void)
{
	BMP_IIC_SCL=0;
	BMP_SDA_OUT();
	BMP_IIC_SDA=0;
	delay_us(2);
	BMP_IIC_SCL=1;
	delay_us(2);
	BMP_IIC_SCL=0;
}
//������ACKӦ��		    
void BMP_IIC_NAck(void)
{
	BMP_IIC_SCL=0;
	BMP_SDA_OUT();
	BMP_IIC_SDA=1;
	delay_us(2);
	BMP_IIC_SCL=1;
	delay_us(2);
	BMP_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void BMP_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	BMP_SDA_OUT(); 	    
    BMP_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        BMP_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		BMP_IIC_SCL=1;
		delay_us(2); 
		BMP_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 BMP_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	BMP_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        BMP_IIC_SCL=0; 
        delay_us(2);
		BMP_IIC_SCL=1;
        receive<<=1;
        if(BMP_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        BMP_IIC_NAck();//����nACK
    else
        BMP_IIC_Ack(); //����ACK   
    return receive;
}



























