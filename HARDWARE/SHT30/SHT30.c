#include "SHT30.h"

float humiture[4];
u8 humiture_buff1[20];

float Temperature;
 float Humidity;
//初始化
void SHT30_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
 	SHT30_SCL=1;
	SHT30_SDA_OUT=1;
}
/*主机发送ACK*/
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
 /*主机不发送ACK*/
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
 /*主机等待从机的ACK*/
u8 SHT30_IIC_wait_ACK(void)
{
    u8 t = 200;
    SHT30_IIC_OUTPUT_MODE_SET();
    SHT30_SDA_OUT=1;//8位发送完后释放数据线，准备接收应答位 
    delay_us(1);
    SHT30_SCL=0;
    delay_us(1); 
    SHT30_IIC_INPUT_MODE_SET();
    delay_us(1); 
    while(SHT30_SDA_IN)//等待SHT30应答
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
功能:启动I2C总线,即发送I2C起始条件.  
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
功能:结束I2C总线,即发送I2C结束条件.  
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
字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:将数据c发送出去,可以是地址,也可以是数据
********************************************************************/
void  SHT30_IIC_SendByte(u8 byte)
{
	u8  BitCnt;
	SHT30_IIC_OUTPUT_MODE_SET();
	SHT30_SCL=0;
	for(BitCnt=0;BitCnt<8;BitCnt++)//要传送的数据长度为8位
	{
		if(byte&0x80) SHT30_SDA_OUT=1;//判断发送位
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
 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能: 用来接收从器件传来的数据  
********************************************************************/    
u8 SHT30_IIC_RcvByte(void)
{
  u8 retc;
  u8 BitCnt;
  retc=0; 
  SHT30_IIC_INPUT_MODE_SET();//置数据线为输入方式
  delay_us(1);                    
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {  
	SHT30_SCL=0;//置时钟线为低，准备接收数据位
	delay_us(2);               
	SHT30_SCL=1;//置时钟线为高使数据线上数据有效                
	retc=retc<<1;
	if(SHT30_SDA_IN) retc |=1;//读数据位,接收的数据位放入retc中 
	delay_us(1);
  }
  SHT30_SCL=0;    
  return(retc);
}
/*******************************************************************
 温湿度获取函数               
函数原型: SHT30_read_result(u8 addr);
功能: 用来接收从器件采集并合成温湿度
********************************************************************/ 
void SHT30_read_result(u8 addr)
{
	u16 tem,hum;
	u16 buff[6];

	SHT30_IIC_Start();
	SHT30_IIC_SendByte(addr<<1 | write);//写7位I2C设备地址加0作为写取位,1为读取位
	SHT30_IIC_wait_ACK();
	SHT30_IIC_SendByte(0x2C);
	SHT30_IIC_wait_ACK();
	SHT30_IIC_SendByte(0x06);
	SHT30_IIC_wait_ACK();
	SHT30_IIC_Stop();
	delay_ms(50);
	SHT30_IIC_Start();
	SHT30_IIC_SendByte(addr<<1 | read);//写7位I2C设备地址加0作为写取位,1为读取位
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
	
	tem = ((buff[0]<<8) | buff[1]);//温度拼接
	hum = ((buff[3]<<8) | buff[4]);//湿度拼接
	
	/*转换实际温度*/
	Temperature= (175.0f*(float)tem/65535.0f-45.0f) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity= (100.0f*(float)hum/65535.0f);// RH = hum*100 / (2^16-1)
	
//	if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤错误数据
//	{
////		humiture[0]=Temperature;
////		humiture[2]=Humidity;
//		sprintf(humiture_buff1,"%0.1f*C %0.1f%%",Temperature,Humidity);//111.01*C 100.01%（保留2位小数）
//	}
//	printf("温湿度：%s\n",humiture_buff1);
//	hum=0;
//	tem=0;
			
	//printf("Temperature=%0.1f*C Humidity=%0.1f%%\r\n",Temperature,Humidity);
}



