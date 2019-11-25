#ifndef __HDC1080_IIC_H__
#define __HDC1080_IIC_H__
#include "stm32f0xx.h"

#define HDC_IIC_RCC            RCC_AHBPeriph_GPIOA
#define HDC_IIC_PORT           GPIOA
#define HDC_IIC_SCL_PIN        GPIO_Pin_6
#define HDC_IIC_SDA_PIN        GPIO_Pin_5

//IO方向设置
#define HDC_SDA_IN()  {HDC_IIC_PORT->MODER&=~(3<<(10*2));HDC_IIC_PORT->MODER|=0<<10*2;}	  //PA5输入模式
#define HDC_SDA_OUT() {HDC_IIC_PORT->MODER&=~(3<<(10*2));HDC_IIC_PORT->MODER|=1<<10*2;}   //PA5输出模式

//IO操作函数	 
#define HDC_IIC_SCL_0    GPIO_ResetBits(HDC_IIC_PORT,HDC_IIC_SCL_PIN)
#define HDC_IIC_SCL_1    GPIO_SetBits(HDC_IIC_PORT,HDC_IIC_SCL_PIN)
#define HDC_IIC_SDA_0    GPIO_ResetBits(HDC_IIC_PORT,HDC_IIC_SDA_PIN)
#define HDC_IIC_SDA_1    GPIO_SetBits(HDC_IIC_PORT,HDC_IIC_SDA_PIN)
#define HDC_READ_SDA     GPIO_ReadInputDataBit(HDC_IIC_PORT,HDC_IIC_SDA_PIN)

//IIC所有操作函数
void HDC_IIC_Init(void);                //初始化IIC的IO口				 
void HDC_IIC_Start(void);				//发送IIC开始信号
void HDC_IIC_Stop(void);	  			//发送IIC停止信号
void HDC_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t HDC_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t HDC_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void HDC_IIC_Ack(void);					//IIC发送ACK信号
void HDC_IIC_NAck(void);				//IIC不发送ACK信号

uint8_t HDC_IIC_MemWrite(uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t *Txbuf, uint8_t ByteCnt);
uint8_t HDC_IIC_MemRead(uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t *Rxbuf, uint8_t ByteCnt);
#endif
