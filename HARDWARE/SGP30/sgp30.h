#ifndef __HX711_H
#define __HX711_H
#include "sys.h"
 	
#define SGP30_read  0xb1  //SGP30�Ķ���ַ
#define SGP30_write 0xb0  //SGP30��д��ַ
extern u32 CO2Data;
extern u32	TVOCData;//����CO2Ũ�ȱ�����TVOCŨ�ȱ���

//IO��������
#define SGP30_SDA_IN()  {GPIOF->MODER&=~(3<<(7*2));GPIOF->MODER|=0<<7*2;}	//PF7����ģʽ
#define SGP30_SDA_OUT() {GPIOF->MODER&=~(3<<(7*2));GPIOF->MODER|=1<<7*2;} //PF7���ģʽ
//IO��������	 
#define SGP30_IIC_SCL    PFout(6) //SCL
#define SGP30_IIC_SDA    PFout(7) //SDA	 
#define SGP30_READ_SDA   PFin(7)  //����SDA 

//IIC���в�������
void SGP30_IIC_Init(void);                //��ʼ��IIC��IO��				 
void SGP30_IIC_Start(void);				//����IIC��ʼ�ź�
void SGP30_IIC_Stop(void);	  			//����IICֹͣ�ź�
void SGP30_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 SGP30_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 SGP30_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void SGP30_IIC_Ack(void);					//IIC����ACK�ź�
void SGP30_IIC_NAck(void);				//IIC������ACK�ź�

void SGP30_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 SGP30_IIC_Read_One_Byte(u8 daddr,u8 addr);	
void SGP30_Init(void);				  
void SGP30_Write(u8 a, u8 b);
u32 SGP30_Read(void);
void  SGP30_Read_Value(void);


#endif

