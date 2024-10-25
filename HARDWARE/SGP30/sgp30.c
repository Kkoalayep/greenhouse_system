#include "sgp30.h"
#include "delay.h"
#include "usart.h"
u32 CO2Data,TVOCData;//����CO2Ũ�ȱ�����TVOCŨ�ȱ���

//��ʼ��IIC
void SGP30_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	SGP30_IIC_SCL=1;
	SGP30_IIC_SDA=1;
}
//����IIC��ʼ�ź�
void SGP30_IIC_Start(void)
{
	SGP30_SDA_OUT();     //sda�����
	SGP30_IIC_SDA=1;	  	  
	SGP30_IIC_SCL=1;
	delay_us(4);
 	SGP30_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SGP30_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void SGP30_IIC_Stop(void)
{
	SGP30_SDA_OUT();//sda�����
	SGP30_IIC_SCL=0;
	SGP30_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SGP30_IIC_SCL=1; 
	SGP30_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 SGP30_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SGP30_SDA_IN();      //SDA����Ϊ����  
	SGP30_IIC_SDA=1;delay_us(1);	   
	SGP30_IIC_SCL=1;delay_us(1);	 
	while(SGP30_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SGP30_IIC_Stop();
			return 1;
		}
	}
	SGP30_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void SGP30_IIC_Ack(void)
{
	SGP30_IIC_SCL=0;
	SGP30_SDA_OUT();
	SGP30_IIC_SDA=0;
	delay_us(2);
	SGP30_IIC_SCL=1;
	delay_us(2);
	SGP30_IIC_SCL=0;
}
//������ACKӦ��		    
void SGP30_IIC_NAck(void)
{
	SGP30_IIC_SCL=0;
	SGP30_SDA_OUT();
	SGP30_IIC_SDA=1;
	delay_us(2);
	SGP30_IIC_SCL=1;
	delay_us(2);
	SGP30_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void SGP30_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SGP30_SDA_OUT(); 	    
    SGP30_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        SGP30_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		SGP30_IIC_SCL=1;
		delay_us(2); 
		SGP30_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 SGP30_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SGP30_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SGP30_IIC_SCL=0; 
        delay_us(2);
		SGP30_IIC_SCL=1;
        receive<<=1;
        if(SGP30_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        SGP30_IIC_NAck();//����nACK
    else
        SGP30_IIC_Ack(); //����ACK   
    return receive;
}





//��ʼ��IIC�ӿ�
void SGP30_Init(void)
{
	u32 sgp30_dat;
  SGP30_IIC_Init();
  SGP30_Write(0x20, 0x03);
//	SGP30_ad_write(0x20,0x61);
//	SGP30_ad_write(0x01,0x00);
	
		SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;
	TVOCData = sgp30_dat & 0x0000ffff;	
	//SGP30ģ�鿪����Ҫһ��ʱ���ʼ�����ڳ�ʼ���׶ζ�ȡ��CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�Һ㶨���䣬����ϵ��ÿ��һ��ʱ���ȡһ��
	//SGP30ģ���ֵ�����CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�����͡����ڼ����...����ֱ��SGP30ģ���ʼ����ɡ�
	//��ʼ����ɺ�տ�ʼ�������ݻᲨ���Ƚϴ�������������һ��ʱ�����������ȶ���
	//�����ഫ�����Ƚ������ܻ���Ӱ�죬�������ݳ��ֲ����������ģ�����������˲�������
	while(CO2Data == 400 && TVOCData == 0)
	{
		SGP30_Write(0x20,0x08);
		sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
		TVOCData = sgp30_dat & 0x0000ffff;			 //ȡ��TVOCֵ
		printf("SGP30���ڼ����...\r\n");
		delay_ms(500);
	}
}


void SGP30_Write(u8 a, u8 b)
{
  SGP30_IIC_Start();
  SGP30_IIC_Send_Byte(SGP30_write); //����������ַ+дָ��
  SGP30_IIC_Wait_Ack();
  SGP30_IIC_Send_Byte(a);		//���Ϳ����ֽ�
  SGP30_IIC_Wait_Ack();
  SGP30_IIC_Send_Byte(b);
  SGP30_IIC_Wait_Ack();
  SGP30_IIC_Stop();
  delay_ms(100);
}

u32 SGP30_Read(void)
{
  u32 dat;
  u8 crc;
  SGP30_IIC_Start();
  SGP30_IIC_Send_Byte(SGP30_read); //����������ַ+��ָ��
  SGP30_IIC_Wait_Ack();
  dat = SGP30_IIC_Read_Byte(1);
  dat <<= 8;
  dat += SGP30_IIC_Read_Byte(1);
  crc = SGP30_IIC_Read_Byte(1); //crc���ݣ���ȥ
  crc = crc;  //Ϊ�˲��ó��ֱ��뾯��
  dat <<= 8;
  dat += SGP30_IIC_Read_Byte(1);
  dat <<= 8;
  dat += SGP30_IIC_Read_Byte(0);
  SGP30_IIC_Stop();
  return(dat);
}


void  SGP30_Read_Value(void)
{
	u32 sgp30_dat;
	SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
	TVOCData = sgp30_dat & 0x0000ffff;       //ȡ��TVOCֵ
	//printf("CO2:%dppm TVOC:%dppd\r\n",CO2Data,TVOCData);
}

