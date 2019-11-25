#ifndef __HDC_1080_H__
#define __HDC_1080_H__

#include "stm32f0xx.h"

#define         HDC_1080_ADD                            0x40
#define         Configuration_register_add              0x02
#define         Temperature_register_add                0x00
#define         Humidity_register_add                   0x01


typedef enum
{
  Temperature_Resolution_14_bit = 0,
  Temperature_Resolution_11_bit = 1
}Temp_Reso;

typedef enum
{
  Humidity_Resolution_14_bit = 0,
  Humidity_Resolution_11_bit = 1,
  Humidity_Resolution_8_bit =2
}Humi_Reso;

//extern uint8_t HDC_Temp;
//extern uint8_t HDC_Humi;

void HDC1080_Init(void);
void HDC1080_Startup(Temp_Reso Temperature_Resolution_x_bit,Humi_Reso Humidity_Resolution_x_bit);
uint8_t HDC1080_start_measurement(float* temperature, uint8_t* humidity);
void HDC1080DataShow(uint8_t *HDC_Temp, uint8_t *HDC_Humi);
#endif
