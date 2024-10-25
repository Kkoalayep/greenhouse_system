#ifndef __SHT30_H__
#define __SHT30_H__
#include "delay.h"
#include "sys.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"	   

#define write 0
#define read  1

//IIC总线接口定义
#define SHT30_SCL PBout(6)
#define SHT30_SDA_OUT PBout(7)
#define SHT30_SDA_IN PBin(7)
#define SHT30_IIC_INPUT_MODE_SET()   {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7输入模式
#define SHT30_IIC_OUTPUT_MODE_SET()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7输出模式

extern u8 humiture_buff1[20];
extern float Temperature ;
extern float Humidity ;
void SHT30_Init(void);
void SHT30_IIC_ACK(void);
void SHT30_IIC_NACK(void);
u8 SHT30_IIC_wait_ACK(void);
void SHT30_IIC_Start(void);
void SHT30_IIC_Stop(void);
void SHT30_IIC_SendByte(u8 byte);
u8 SHT30_IIC_RcvByte(void);
void SHT30_read_result(u8 addr);
#endif
