#ifndef __HDC1080_IIC_H__
#define __HDC1080_IIC_H__
#include "stm32f0xx.h"

#define HDC_IIC_RCC            RCC_AHBPeriph_GPIOA
#define HDC_IIC_PORT           GPIOA
#define HDC_IIC_SCL_PIN        GPIO_Pin_6
#define HDC_IIC_SDA_PIN        GPIO_Pin_5

//IO��������
#define HDC_SDA_IN()  {HDC_IIC_PORT->MODER&=~(3<<(10*2));HDC_IIC_PORT->MODER|=0<<10*2;}	  //PA5����ģʽ
#define HDC_SDA_OUT() {HDC_IIC_PORT->MODER&=~(3<<(10*2));HDC_IIC_PORT->MODER|=1<<10*2;}   //PA5���ģʽ

//IO��������	 
#define HDC_IIC_SCL_0    GPIO_ResetBits(HDC_IIC_PORT,HDC_IIC_SCL_PIN)
#define HDC_IIC_SCL_1    GPIO_SetBits(HDC_IIC_PORT,HDC_IIC_SCL_PIN)
#define HDC_IIC_SDA_0    GPIO_ResetBits(HDC_IIC_PORT,HDC_IIC_SDA_PIN)
#define HDC_IIC_SDA_1    GPIO_SetBits(HDC_IIC_PORT,HDC_IIC_SDA_PIN)
#define HDC_READ_SDA     GPIO_ReadInputDataBit(HDC_IIC_PORT,HDC_IIC_SDA_PIN)

//IIC���в�������
void HDC_IIC_Init(void);                //��ʼ��IIC��IO��				 
void HDC_IIC_Start(void);				//����IIC��ʼ�ź�
void HDC_IIC_Stop(void);	  			//����IICֹͣ�ź�
void HDC_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t HDC_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t HDC_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void HDC_IIC_Ack(void);					//IIC����ACK�ź�
void HDC_IIC_NAck(void);				//IIC������ACK�ź�

uint8_t HDC_IIC_MemWrite(uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t *Txbuf, uint8_t ByteCnt);
uint8_t HDC_IIC_MemRead(uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t *Rxbuf, uint8_t ByteCnt);
#endif
