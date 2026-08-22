#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "main.h"

typedef enum
{
	Input,
	Output
} PinMode;

typedef enum
{
	DS18B20_NORMAL=0,
	DS18B20_SHORTCIRCUIT
}DS18B20_STATUS_TYPE;

//各部分温度数据
typedef struct DS18B20_Temperature
{
	int16_t temp_c;
	int16_t temp_f;
	DS18B20_STATUS_TYPE status;
} DS18B20_Dev_t;

extern DS18B20_Dev_t DS18B20_Dev;



#define DS18B20_IO_Read()    HAL_GPIO_ReadPin(DS18B20_IO_GPIO_Port, DS18B20_IO_Pin)
#define DS18B20_IO_Write_1() HAL_GPIO_WritePin(DS18B20_IO_GPIO_Port, DS18B20_IO_Pin, GPIO_PIN_SET)
#define DS18B20_IO_Write_0() HAL_GPIO_WritePin(DS18B20_IO_GPIO_Port, DS18B20_IO_Pin, GPIO_PIN_RESET)


void DS18B20_Init(void);
void DS18B20_GetTemperature(void);
void DS18B20_Test(void);

#endif //__DS18B20_H__
