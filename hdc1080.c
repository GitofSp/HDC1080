#include "hdc1080.h"
#include "hdc1080_iic.h"
#include "delay.h"
#include <stdio.h>
#include "server.h"

volatile float temp;
volatile uint8_t humi;

void HDC1080_Init(void)
{
	HDC_IIC_Init();
}

void HDC1080_Startup(Temp_Reso Temperature_Resolution_x_bit,Humi_Reso Humidity_Resolution_x_bit)
{
	/* Temperature and Humidity are acquired in sequence, Temperature first
	 * Default:   Temperature resolution = 14 bit,
	 *            Humidity resolution = 14 bit
	 */

	/* Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1 */
	uint16_t config_reg_value=0x3000;
	uint8_t data_send[2];
	if(Temperature_Resolution_x_bit == Temperature_Resolution_11_bit)
	{
		config_reg_value |= (1<<10); //11 bit
	}

	switch((uint8_t)Humidity_Resolution_x_bit)
	{
		case Humidity_Resolution_11_bit:
			config_reg_value|= (1<<8);
			break;
		case Humidity_Resolution_8_bit:
			config_reg_value|= (1<<9);
			break;
	}

	data_send[0]= (config_reg_value>>8);
	data_send[1]= (config_reg_value&0x00ff);
	
	HDC_IIC_MemWrite(HDC_1080_ADD,Configuration_register_add,data_send,2);
}

uint8_t HDC1080_start_measurement(float* temperature, uint8_t* humidity)
{
	uint8_t receive_data[4];
	uint16_t temp_x,humi_x;

	HDC_IIC_MemRead(HDC_1080_ADD, Temperature_register_add, receive_data, 2);
	temp_x =((receive_data[0]<<8)|receive_data[1]);
	
	HDC_IIC_MemRead(HDC_1080_ADD, Humidity_register_add, receive_data, 2);
	humi_x =((receive_data[0]<<8)|receive_data[1]);

	*temperature=((((float)temp_x)/65536.0)*165.0)-40.0;
	
	*humidity=(uint8_t)((humi_x/65536.0)*100.0);

	return 0;
}
void HDC1080DataShow(uint8_t *HDC_Temp, uint8_t *HDC_Humi)
{
	if(!HDC1080_start_measurement((float*)&temp,(uint8_t*)&humi)){
		printf("当前环境温度   : %0.1f °C\r\n",temp);
		printf("当前环境湿度   : %0.2d\r\n",humi);
		*HDC_Temp = (uint8_t)temp;
		*HDC_Humi = humi;
	}
}

