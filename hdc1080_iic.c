#include "hdc1080_iic.h"
#include "delay.h"
#include <stdio.h>

//初始化IIC
void HDC_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(HDC_IIC_RCC, ENABLE);//使能GPIO的外设时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = HDC_IIC_SCL_PIN | HDC_IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(HDC_IIC_PORT, &GPIO_InitStructure);//初始化
	HDC_IIC_SCL_1;
	HDC_IIC_SDA_1;
}

//产生IIC起始信号
void HDC_IIC_Start(void)
{
	HDC_SDA_OUT();     //sda线输出
	HDC_IIC_SDA_1;	  	
	HDC_IIC_SCL_1;
	delay_us(4);
 	HDC_IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	HDC_IIC_SCL_0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void HDC_IIC_Stop(void)
{
	HDC_SDA_OUT();//sda线输出
	HDC_IIC_SCL_0;
	HDC_IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	HDC_IIC_SCL_1; 
	HDC_IIC_SDA_1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t HDC_IIC_Wait_Ack(void)
{
	uint16_t ucErrTime=0,res = 0;
	HDC_SDA_IN(); delay_us(1);   //SDA设置为输入  
	while(HDC_READ_SDA)
	{
		ucErrTime++;
		delay_us(1);
		if(ucErrTime>250)
		{
			res = 1;
			break;
		}
	}

	HDC_IIC_SCL_1;delay_us(1);	
	HDC_IIC_SCL_0;delay_us(1);	   
	return res;  
} 
//产生ACK应答
void HDC_IIC_Ack(void)
{
	HDC_IIC_SCL_0;
	HDC_SDA_OUT();
	HDC_IIC_SDA_0;
	delay_us(2);
	HDC_IIC_SCL_1;
	delay_us(2);
	HDC_IIC_SCL_0;
}
//不产生ACK应答		    
void HDC_IIC_NAck(void)
{
	HDC_IIC_SCL_0;
	HDC_SDA_OUT();
	HDC_IIC_SDA_1;
	delay_us(2);
	HDC_IIC_SCL_1;
	delay_us(2);
	HDC_IIC_SCL_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void HDC_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	HDC_SDA_OUT(); 	    
    HDC_IIC_SCL_0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if(((txd&0x80)>>7)){
			HDC_IIC_SDA_1;
		}else{
			HDC_IIC_SDA_0;
		}
        txd<<=1; 	  
		delay_us(2);   
		HDC_IIC_SCL_1;
		delay_us(2); 
		HDC_IIC_SCL_0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t HDC_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	
	HDC_SDA_IN();//SDA设置为输入
	
    for(i=0;i<8;i++ )
	{
        HDC_IIC_SCL_0; 
        delay_us(2);
		HDC_IIC_SCL_1;
        receive<<=1;
        if(HDC_READ_SDA)receive++;   
		delay_us(1); 
		
    }	
    if (!ack)
        HDC_IIC_NAck();//发送nACK
    else
        HDC_IIC_Ack(); //发送ACK
    return receive;
}

uint8_t HDC_IIC_MemWrite(uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t *Txbuf, uint8_t ByteCnt)
{
	uint8_t i,ret = 0;
	
	HDC_IIC_Start(); 
	
	HDC_IIC_Send_Byte((SlaveAddr<<1)|0x00);
	if(HDC_IIC_Wait_Ack()){
		printf("HDC_IIC_MemWrite 1 ack err\r\n");
		ret = 1;
	}
	
	HDC_IIC_Send_Byte(RegisterAddr);
	if(HDC_IIC_Wait_Ack()){
		printf("HDC_IIC_MemWrite 2 ack err\r\n");
		ret = 2;
	}
	
	for(i=0;i<ByteCnt;i++){
		HDC_IIC_Send_Byte(Txbuf[i]);
		HDC_IIC_Wait_Ack();
	}
	
	HDC_IIC_Stop(); 
	delay_ms(10);
	return ret;
}

uint8_t HDC_IIC_MemRead(uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t *Rxbuf, uint8_t ByteCnt)
{

	uint8_t i=0,ret = 0;
	
	HDC_IIC_Start(); 

	/* 设备地址 写 */
	HDC_IIC_Send_Byte((SlaveAddr<<1)|0x00);
	if(HDC_IIC_Wait_Ack()){
		ret = 1;
	}
	
	/* 寄存器地址 写*/
	HDC_IIC_Send_Byte(RegisterAddr);
	if(HDC_IIC_Wait_Ack()){
		ret = 2;
	}
	
	delay_ms(15);
	
	HDC_IIC_Start();
	/* 设备地址 读 */
	HDC_IIC_Send_Byte((SlaveAddr<<1)|0x01);
	if(HDC_IIC_Wait_Ack()){
		ret = 3;
	}
	
	/* 数据 */
	for(i=0;i<ByteCnt-1;i++){
		Rxbuf[i] = HDC_IIC_Read_Byte(1);
	}
	Rxbuf[ByteCnt-1] = HDC_IIC_Read_Byte(0);
	HDC_IIC_Stop(); 

	delay_ms(20);
	return ret;
}

