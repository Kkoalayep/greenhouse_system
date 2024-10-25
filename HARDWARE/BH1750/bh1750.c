#include "BH1750.h"
#include "delay.h"
#include "sys.h"
#include <stdint.h>
#include "usart.h"

u8 BUF[8];
u16 temp2=0;
float lux;


//初始化IIC
void BH1750_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	BH1750_IIC_SCL=1;
	BH1750_IIC_SDA=1;
}
/***起始信号***/
void BH1750_IIC_Start(void)
{
		BH1750_SDA_OUT()       //设置SDA线为输出
	//在开始数据传输前，先让SDA和SCL都拉高
		BH1750_IIC_SDA=1;    //发送起始条件的数据信号，释放总线
	  delay_us(2);
		BH1750_IIC_SCL=1;    

		delay_us(5);   //Tsu;STA：起始条件的建立时间大于4.7us。
	
		BH1750_IIC_SDA=0;    //SDA由高变为低表示开始信号
		delay_us(4);   //起始条件的保持时间大于4us
		BH1750_IIC_SCL=0;    //钳住IIC总线，准备发送或者接收数据
		delay_us(2);
}

/***停止信号***/
void BH1750_IIC_Stop(void)
{
	BH1750_SDA_OUT()       //设置SDA线为输出
	
	//IIC_SCL=0;    //此句是调试BH1750时加，2018.03.03
	
	BH1750_IIC_SDA=0;   //发送停止信号的数据信号
	delay_us(2);
	BH1750_IIC_SCL=1;   //发送停止信号的时钟信号
	delay_us(5);  //停止信号的建立时间大于4us
	BH1750_IIC_SDA=1;   //发送停止信号
	delay_us(4);    //
	
}


/****等待应答信号到来****/
/*从器件进行应答信号
返回值：1 接收应答失败
         0  接收应答成功
注意;此函数的返回值不会对下面的语句才生影响
*/
u8 BH1750_IIC_Wait_Ack(void)
{
	u8 ucErrorTime=0;
	BH1750_SDA_IN();     //SDA设置成输入
	BH1750_IIC_SDA=1;
	delay_us(2);
	BH1750_IIC_SCL=1;
	delay_us(2);
	while(BH1750_READ_SDA)
	{
		ucErrorTime++;
		if(ucErrorTime>=250)
		{
			BH1750_IIC_Stop();
			return 1;		
		}
	}
	BH1750_IIC_SCL=0;
	return 0;
}

/****产生Ack应答****/
/****
       主器件进行应答信号，产生应答
****/
void BH1750_IIC_Ack(void)
{
	BH1750_IIC_SCL=0;
	BH1750_SDA_OUT();
	BH1750_IIC_SDA=0;   //主器件应答
	delay_us(2);
	BH1750_IIC_SCL=1;
	delay_us(4);        //SCL高电平周期大于4us
	BH1750_IIC_SCL=0;         //清时钟线，钳住IIC总线以便继续接收
	delay_us(2);
}
/****不产生Ack应答****/
/****
       主器件进行应答信号，不产生应答
****/
void BH1750_IIC_NAck(void)
{
	BH1750_IIC_SCL=0;
	BH1750_SDA_OUT();
	BH1750_IIC_SDA=1;   //主器件不应答
	delay_us(2);
	BH1750_IIC_SCL=1;
	delay_us(4);        //SCL高电平周期大于4us
	BH1750_IIC_SCL=0;         //清时钟线，钳住IIC总线以便继续接收
	delay_us(2);
}


/****发送一个字节****/
/****
功能:将数据c发送出去，可以是地址，也可以是数据，发送完后等待应答，
****/
void BH1750_IIC_Send_Byte(unsigned char c)  //要传送的数据长度为8位
{
	u8 i;
	BH1750_SDA_OUT();  

	for(i=0;i<8;i++)
	{
		if((c<<i)&0x80)  BH1750_IIC_SDA=1;   //判断发送位
		else BH1750_IIC_SDA=0;

		delay_us(2);
		BH1750_IIC_SCL=1;     //拉高SCL，通知被控器开始接收数据位
		delay_us(4);   //保证SCL高电平周期大于4us
		BH1750_IIC_SCL=0;    //拉低SCL，允许SDA传输下一位数据。因为只有在SCL=0时才允许SDA的状态发生变化
	  delay_us(2);
   }
	 //以下四句是调试BH1750时添加的，2018.03.03
	 //特别注意;用方法三时，以下四句不可去掉，去掉后得到的数有问题
	 //用方法二时，以下四句需注释掉，否则也会有问题，至于原因暂时还不清楚。
// 	 IIC_SDA=0;
// 	 delay_us(2);
// 	 IIC_SCL=1;
// 	 delay_us(2);
	 
	 BH1750_IIC_SCL=0;      //拉低SCL，为下次数据传输做好准备
	 delay_us(2);
}


/***接收一个字节***/
/***
功能:用来接收从器件传来的数据，ack为1时，发送Ack(产生应答);ack为0时，发送NAck(不产生应答)
注意：IIC读写数据时，高位在前，低位在后
***/
u8 BH1750_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	BH1750_SDA_IN();    
	for(i=0;i<8;i++)
	{
		BH1750_IIC_SCL=0;    //置SCL为低，准备接收数据位
		delay_us(5);  //时钟低电平周期大于4.7us
		BH1750_IIC_SCL=1;   //置SCL为高，使SDA上数据有效
		delay_us(2);
		receive=receive<<1;
		if(BH1750_READ_SDA) receive=receive+1;    //读取SDA，把接收的数据位放入receve中，若此处写成IIC_SDA==1，会失败，思考原因？	
		delay_us(2);
  }
	if(!ack) BH1750_IIC_NAck();   //发送NAck
	else BH1750_IIC_Ack();    //发送Ack
	//IIC_SCL=0;      //在此处有何用处？？？(猜测：为下次数据传输做好准备， //钳住IIC总线，准备发送或者接收数据)
	//delay_us(2);
	return receive;

}


/****向BH1750单次写命令****/
void Single_Write_BH1750(unsigned char Reg_Address)
{
	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(0x46);  //发送器件地址0100 0110   最后一位0，表示写
	//IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(Reg_Address);  
	//IIC_Wait_Ack();
	BH1750_IIC_Stop();
}


/****BH1750初始化****/
void BH1750_Init(void)
{
	BH1750_IIC_Init();
  Single_Write_BH1750(0x01);
	
}


/****从BH1750单次读Lux****/
/****
模式选择：H-Resolution Mode,分辨率1lux
单次读Opecode：0010 0000 ，即0x20
器件地址：0100011，ADDR接GND
****/
void BH1750_ReadOneTime(void)
{
	

	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(0x46);  //发送器件地址0100 0110   最后一位0，表示写
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(0x20);   //发送One time H-Resolution Mode的Opecode 0010 0000，分辨率为1lux
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
	delay_ms(200);
	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(0x47);  //
	BH1750_IIC_Wait_Ack();
	BUF[0]=BH1750_IIC_Read_Byte(1);
	//IIC_Ack();        //主机应答
	//temp1=temp<<8;
	BUF[1]=BH1750_IIC_Read_Byte(0);
	//IIC_NAck();        //主机不应答	
	BH1750_IIC_Stop();
	
	
// 	temp2=BUF[0];
// 	temp2=(temp2<<8)+BUF[1];
// 	lux=(float)temp2/1.2;    //lux是float型
	
	//return lux;
	//delay_ms(200);
}

/****方法一：****/
/****从BH1750连续读Lux****/
/****
模式选择：H-Resolution Mode,分辨率1lux
连续读Opecode：0001 0000
器件地址：0100011，ADDR接GND
****/
void  BH1750_ReadContinuous1(void)
{
	u16 temp=0,temp1=0;


	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x46);  //发送器件地址0100 0110   最后一位0，表示写
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(0x10);  //发送Continuous H-Resolution Mode的Opecode 0001 0000，分辨率为1lux
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
	delay_ms(200);
	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x47);  //此处严重失误，写成了IIC_Read_Byte,想当然的认为最后一位是1，就应该用IIC_Read_Byte，特别注意！！！
	BH1750_IIC_Wait_Ack();
	
	
// 	BUF[0]=IIC_Read_Byte(1);
// 	BUF[1]=IIC_Read_Byte(0);	
	
	
	temp=BH1750_IIC_Read_Byte(1);
	temp1=BH1750_IIC_Read_Byte(0);


	BH1750_IIC_Stop();
	
	temp2=temp1+(temp<<8);
	lux=(float)temp2/1.2f;    //lux是float型


}


/****方法二：****/


void  BH1750_ReadContinuous2(void)
{
	//u16 temp=0,temp1=0;


	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x46);  //发送器件地址0100 0110   最后一位0，表示写
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(0x10);  //发送Continuous H-Resolution Mode的Opecode 0001 0000，分辨率为1lux
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
	delay_ms(200);
	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x47);  //此处严重失误，写成了IIC_Read_Byte,想当然的认为最后一位是1，就应该用IIC_Read_Byte，特别注意！！！
	BH1750_IIC_Wait_Ack();
	
	
	BUF[0]=BH1750_IIC_Read_Byte(1);
	BUF[1]=BH1750_IIC_Read_Byte(0);	
	
	
// 	temp=IIC_Read_Byte(1);
// 	temp1=IIC_Read_Byte(0);

	BH1750_IIC_Stop();
	
// 	temp2=temp1+(temp<<8);
// 	lux=(float)temp2/1.2;    //lux是float型


}






/****方法三****/
/****
利用函数BH1750_Multiple_Read()进行读
利用Conversion(void)函数进行转换
****/
void BH1750_Multiple_Read(void)
{
	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x47);  //此处严重失误，写成了IIC_Read_Byte,想当然的认为最后一位是1，就应该用IIC_Read_Byte，特别注意！！！

	BUF[0]=BH1750_IIC_Read_Byte(1); 
	BUF[1]=BH1750_IIC_Read_Byte(0); 	
	BH1750_IIC_Stop();                          //停止信号  
	delay_us(5);
	delay_ms(200);
}

void Conversion(void)
{
	temp2=BUF[0];
	temp2=(temp2<<8)+BUF[1];
	lux=(float)temp2/1.2f;    //lux是float型
	
}


void BH1750_Read(void)
{
		BH1750_ReadContinuous1();
	//	printf("light:%0.1f lux\r\n",lux);
}


