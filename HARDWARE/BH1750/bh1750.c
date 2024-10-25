#include "BH1750.h"
#include "delay.h"
#include "sys.h"
#include <stdint.h>
#include "usart.h"

u8 BUF[8];
u16 temp2=0;
float lux;


//��ʼ��IIC
void BH1750_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	BH1750_IIC_SCL=1;
	BH1750_IIC_SDA=1;
}
/***��ʼ�ź�***/
void BH1750_IIC_Start(void)
{
		BH1750_SDA_OUT()       //����SDA��Ϊ���
	//�ڿ�ʼ���ݴ���ǰ������SDA��SCL������
		BH1750_IIC_SDA=1;    //������ʼ�����������źţ��ͷ�����
	  delay_us(2);
		BH1750_IIC_SCL=1;    

		delay_us(5);   //Tsu;STA����ʼ�����Ľ���ʱ�����4.7us��
	
		BH1750_IIC_SDA=0;    //SDA�ɸ߱�Ϊ�ͱ�ʾ��ʼ�ź�
		delay_us(4);   //��ʼ�����ı���ʱ�����4us
		BH1750_IIC_SCL=0;    //ǯסIIC���ߣ�׼�����ͻ��߽�������
		delay_us(2);
}

/***ֹͣ�ź�***/
void BH1750_IIC_Stop(void)
{
	BH1750_SDA_OUT()       //����SDA��Ϊ���
	
	//IIC_SCL=0;    //�˾��ǵ���BH1750ʱ�ӣ�2018.03.03
	
	BH1750_IIC_SDA=0;   //����ֹͣ�źŵ������ź�
	delay_us(2);
	BH1750_IIC_SCL=1;   //����ֹͣ�źŵ�ʱ���ź�
	delay_us(5);  //ֹͣ�źŵĽ���ʱ�����4us
	BH1750_IIC_SDA=1;   //����ֹͣ�ź�
	delay_us(4);    //
	
}


/****�ȴ�Ӧ���źŵ���****/
/*����������Ӧ���ź�
����ֵ��1 ����Ӧ��ʧ��
         0  ����Ӧ��ɹ�
ע��;�˺����ķ���ֵ����������������Ӱ��
*/
u8 BH1750_IIC_Wait_Ack(void)
{
	u8 ucErrorTime=0;
	BH1750_SDA_IN();     //SDA���ó�����
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

/****����AckӦ��****/
/****
       ����������Ӧ���źţ�����Ӧ��
****/
void BH1750_IIC_Ack(void)
{
	BH1750_IIC_SCL=0;
	BH1750_SDA_OUT();
	BH1750_IIC_SDA=0;   //������Ӧ��
	delay_us(2);
	BH1750_IIC_SCL=1;
	delay_us(4);        //SCL�ߵ�ƽ���ڴ���4us
	BH1750_IIC_SCL=0;         //��ʱ���ߣ�ǯסIIC�����Ա��������
	delay_us(2);
}
/****������AckӦ��****/
/****
       ����������Ӧ���źţ�������Ӧ��
****/
void BH1750_IIC_NAck(void)
{
	BH1750_IIC_SCL=0;
	BH1750_SDA_OUT();
	BH1750_IIC_SDA=1;   //��������Ӧ��
	delay_us(2);
	BH1750_IIC_SCL=1;
	delay_us(4);        //SCL�ߵ�ƽ���ڴ���4us
	BH1750_IIC_SCL=0;         //��ʱ���ߣ�ǯסIIC�����Ա��������
	delay_us(2);
}


/****����һ���ֽ�****/
/****
����:������c���ͳ�ȥ�������ǵ�ַ��Ҳ���������ݣ��������ȴ�Ӧ��
****/
void BH1750_IIC_Send_Byte(unsigned char c)  //Ҫ���͵����ݳ���Ϊ8λ
{
	u8 i;
	BH1750_SDA_OUT();  

	for(i=0;i<8;i++)
	{
		if((c<<i)&0x80)  BH1750_IIC_SDA=1;   //�жϷ���λ
		else BH1750_IIC_SDA=0;

		delay_us(2);
		BH1750_IIC_SCL=1;     //����SCL��֪ͨ��������ʼ��������λ
		delay_us(4);   //��֤SCL�ߵ�ƽ���ڴ���4us
		BH1750_IIC_SCL=0;    //����SCL������SDA������һλ���ݡ���Ϊֻ����SCL=0ʱ������SDA��״̬�����仯
	  delay_us(2);
   }
	 //�����ľ��ǵ���BH1750ʱ��ӵģ�2018.03.03
	 //�ر�ע��;�÷�����ʱ�������ľ䲻��ȥ����ȥ����õ�����������
	 //�÷�����ʱ�������ľ���ע�͵�������Ҳ�������⣬����ԭ����ʱ���������
// 	 IIC_SDA=0;
// 	 delay_us(2);
// 	 IIC_SCL=1;
// 	 delay_us(2);
	 
	 BH1750_IIC_SCL=0;      //����SCL��Ϊ�´����ݴ�������׼��
	 delay_us(2);
}


/***����һ���ֽ�***/
/***
����:�������մ��������������ݣ�ackΪ1ʱ������Ack(����Ӧ��);ackΪ0ʱ������NAck(������Ӧ��)
ע�⣺IIC��д����ʱ����λ��ǰ����λ�ں�
***/
u8 BH1750_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	BH1750_SDA_IN();    
	for(i=0;i<8;i++)
	{
		BH1750_IIC_SCL=0;    //��SCLΪ�ͣ�׼����������λ
		delay_us(5);  //ʱ�ӵ͵�ƽ���ڴ���4.7us
		BH1750_IIC_SCL=1;   //��SCLΪ�ߣ�ʹSDA��������Ч
		delay_us(2);
		receive=receive<<1;
		if(BH1750_READ_SDA) receive=receive+1;    //��ȡSDA���ѽ��յ�����λ����receve�У����˴�д��IIC_SDA==1����ʧ�ܣ�˼��ԭ��	
		delay_us(2);
  }
	if(!ack) BH1750_IIC_NAck();   //����NAck
	else BH1750_IIC_Ack();    //����Ack
	//IIC_SCL=0;      //�ڴ˴��к��ô�������(�²⣺Ϊ�´����ݴ�������׼���� //ǯסIIC���ߣ�׼�����ͻ��߽�������)
	//delay_us(2);
	return receive;

}


/****��BH1750����д����****/
void Single_Write_BH1750(unsigned char Reg_Address)
{
	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
	//IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(Reg_Address);  
	//IIC_Wait_Ack();
	BH1750_IIC_Stop();
}


/****BH1750��ʼ��****/
void BH1750_Init(void)
{
	BH1750_IIC_Init();
  Single_Write_BH1750(0x01);
	
}


/****��BH1750���ζ�Lux****/
/****
ģʽѡ��H-Resolution Mode,�ֱ���1lux
���ζ�Opecode��0010 0000 ����0x20
������ַ��0100011��ADDR��GND
****/
void BH1750_ReadOneTime(void)
{
	

	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(0x20);   //����One time H-Resolution Mode��Opecode 0010 0000���ֱ���Ϊ1lux
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
	delay_ms(200);
	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(0x47);  //
	BH1750_IIC_Wait_Ack();
	BUF[0]=BH1750_IIC_Read_Byte(1);
	//IIC_Ack();        //����Ӧ��
	//temp1=temp<<8;
	BUF[1]=BH1750_IIC_Read_Byte(0);
	//IIC_NAck();        //������Ӧ��	
	BH1750_IIC_Stop();
	
	
// 	temp2=BUF[0];
// 	temp2=(temp2<<8)+BUF[1];
// 	lux=(float)temp2/1.2;    //lux��float��
	
	//return lux;
	//delay_ms(200);
}

/****����һ��****/
/****��BH1750������Lux****/
/****
ģʽѡ��H-Resolution Mode,�ֱ���1lux
������Opecode��0001 0000
������ַ��0100011��ADDR��GND
****/
void  BH1750_ReadContinuous1(void)
{
	u16 temp=0,temp1=0;


	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(0x10);  //����Continuous H-Resolution Mode��Opecode 0001 0000���ֱ���Ϊ1lux
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
	delay_ms(200);
	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x47);  //�˴�����ʧ��д����IIC_Read_Byte,�뵱Ȼ����Ϊ���һλ��1����Ӧ����IIC_Read_Byte���ر�ע�⣡����
	BH1750_IIC_Wait_Ack();
	
	
// 	BUF[0]=IIC_Read_Byte(1);
// 	BUF[1]=IIC_Read_Byte(0);	
	
	
	temp=BH1750_IIC_Read_Byte(1);
	temp1=BH1750_IIC_Read_Byte(0);


	BH1750_IIC_Stop();
	
	temp2=temp1+(temp<<8);
	lux=(float)temp2/1.2f;    //lux��float��


}


/****��������****/


void  BH1750_ReadContinuous2(void)
{
	//u16 temp=0,temp1=0;


	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(0x10);  //����Continuous H-Resolution Mode��Opecode 0001 0000���ֱ���Ϊ1lux
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
	delay_ms(200);
	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x47);  //�˴�����ʧ��д����IIC_Read_Byte,�뵱Ȼ����Ϊ���һλ��1����Ӧ����IIC_Read_Byte���ر�ע�⣡����
	BH1750_IIC_Wait_Ack();
	
	
	BUF[0]=BH1750_IIC_Read_Byte(1);
	BUF[1]=BH1750_IIC_Read_Byte(0);	
	
	
// 	temp=IIC_Read_Byte(1);
// 	temp1=IIC_Read_Byte(0);

	BH1750_IIC_Stop();
	
// 	temp2=temp1+(temp<<8);
// 	lux=(float)temp2/1.2;    //lux��float��


}






/****������****/
/****
���ú���BH1750_Multiple_Read()���ж�
����Conversion(void)��������ת��
****/
void BH1750_Multiple_Read(void)
{
	BH1750_IIC_Start();
  BH1750_IIC_Send_Byte(0x47);  //�˴�����ʧ��д����IIC_Read_Byte,�뵱Ȼ����Ϊ���һλ��1����Ӧ����IIC_Read_Byte���ر�ע�⣡����

	BUF[0]=BH1750_IIC_Read_Byte(1); 
	BUF[1]=BH1750_IIC_Read_Byte(0); 	
	BH1750_IIC_Stop();                          //ֹͣ�ź�  
	delay_us(5);
	delay_ms(200);
}

void Conversion(void)
{
	temp2=BUF[0];
	temp2=(temp2<<8)+BUF[1];
	lux=(float)temp2/1.2f;    //lux��float��
	
}


void BH1750_Read(void)
{
		BH1750_ReadContinuous1();
	//	printf("light:%0.1f lux\r\n",lux);
}


