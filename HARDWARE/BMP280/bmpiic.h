#ifndef __BMPIIC_H
#define __BMPIIC_H
#include "sys.h" 
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
   	   		   
//IO��������
#define BMP_SDA_IN()  {GPIOD->MODER&=~(3<<(1*2));GPIOD->MODER|=0<<1*2;}	//PD1����ģʽ
#define BMP_SDA_OUT() {GPIOD->MODER&=~(3<<(1*2));GPIOD->MODER|=1<<1*2;} //PD1���ģʽ
//IO��������	 
#define BMP_IIC_SCL    PDout(0) //SCL
#define BMP_IIC_SDA    PDout(1) //SDA	 
#define BMP_READ_SDA   PDin(1)  //����SDA 

//IIC���в�������
void BMP_IIC_Init(void);                //��ʼ��IIC��IO��				 
void BMP_IIC_Start(void);				//����IIC��ʼ�ź�
void BMP_IIC_Stop(void);	  			//����IICֹͣ�ź�
void BMP_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 BMP_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 BMP_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void BMP_IIC_Ack(void);					//IIC����ACK�ź�
void BMP_IIC_NAck(void);				//IIC������ACK�ź�

void BMP_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 BMP_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















