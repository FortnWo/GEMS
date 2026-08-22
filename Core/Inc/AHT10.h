#ifndef _AHT10_H_
#define _AHT10_H_

#include "main.h"

// 定义AHT10地址ַ
#define AHT10_ADDRESS 0x70
#define AHT10_Write_ADDRESS 0x70
#define AHT10_Read_ADDRESS 0x71

// 定义AHT10命令
#define AHT10_Init_com 0xE1      // 1110 0001
#define AHT10_SoftReset_com 0xBA // 1011 1010
#define AHT10_TrigeMea_com 0xAC  // 1010 1100

typedef enum
{
	AHT10_NORMAL=0,
	AHT10_SHORTCIRCUIT
}AHT10_STATUS_TYPE;

//各部分温度数据
typedef struct
{
	int16_t temp_c;
	int16_t temp_f;
	int16_t humi;
	AHT10_STATUS_TYPE status;
} AHT10_Dev_t;

extern AHT10_Dev_t AHT10_Dev;






void AHT10_Init(void);
uint8_t AHT10_Read_predata(float *humidity, float *temperature);

void AHT10_GetTemperature(void);
void AHT10_Test(void);
#endif

