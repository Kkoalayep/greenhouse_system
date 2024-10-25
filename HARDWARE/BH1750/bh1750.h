#ifndef __BH1750_H
#define __BH1750_H


#include "sys.h"
extern float lux;

//IO��������
#define BH1750_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define BH1750_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
#define BH1750_IIC_SCL    PBout(8) //SCL
#define BH1750_IIC_SDA    PBout(9) //SDA	 
#define BH1750_READ_SDA   PBin(9)  //����SDA 

//IIC���в�������
void BH1750_BH1750_IIC_Init(void);                //��ʼ��IIC��IO��				 
void BH1750_IIC_Start(void);				//����IIC��ʼ�ź�
void BH1750_IIC_Stop(void);	  			//����IICֹͣ�ź�
void BH1750_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 BH1750_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 BH1750_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void BH1750_IIC_Ack(void);					//IIC����ACK�ź�
void BH1750_IIC_NAck(void);				//IIC������ACK�ź�

void BH1750_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 BH1750_IIC_Read_One_Byte(u8 daddr,u8 addr);


void Single_Write_BH1750(unsigned char Reg_Address);
void BH1750_Init(void);
void BH1750_ReadOneTime(void);
void BH1750_ReadContinuous1(void);
void BH1750_ReadContinuous2(void);
void BH1750_Multiple_Read(void);
void Conversion(void);
void BH1750_Read(void);

#endif


