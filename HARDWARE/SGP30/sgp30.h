#ifndef __HX711_H
#define __HX711_H
#include "sys.h"
 	
#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址
extern u32 CO2Data;
extern u32	TVOCData;//定义CO2浓度变量与TVOC浓度变量

//IO方向设置
#define SGP30_SDA_IN()  {GPIOF->MODER&=~(3<<(7*2));GPIOF->MODER|=0<<7*2;}	//PF7输入模式
#define SGP30_SDA_OUT() {GPIOF->MODER&=~(3<<(7*2));GPIOF->MODER|=1<<7*2;} //PF7输出模式
//IO操作函数	 
#define SGP30_IIC_SCL    PFout(6) //SCL
#define SGP30_IIC_SDA    PFout(7) //SDA	 
#define SGP30_READ_SDA   PFin(7)  //输入SDA 

//IIC所有操作函数
void SGP30_IIC_Init(void);                //初始化IIC的IO口				 
void SGP30_IIC_Start(void);				//发送IIC开始信号
void SGP30_IIC_Stop(void);	  			//发送IIC停止信号
void SGP30_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 SGP30_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 SGP30_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void SGP30_IIC_Ack(void);					//IIC发送ACK信号
void SGP30_IIC_NAck(void);				//IIC不发送ACK信号

void SGP30_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 SGP30_IIC_Read_One_Byte(u8 daddr,u8 addr);	
void SGP30_Init(void);				  
void SGP30_Write(u8 a, u8 b);
u32 SGP30_Read(void);
void  SGP30_Read_Value(void);


#endif

