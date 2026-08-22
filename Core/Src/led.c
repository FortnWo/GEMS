/*
 * led.c
 *
 *  Created on: Sep 4, 2024
 *      Author: dongjundj, FORTUNE
 */

#include "led.h"

// 初始化结构体结构体数据类型
typedef struct Led_gpio_t
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}LED_GPIO_T;


// 定义一个静态全局变量保存GPIO口的资源信息
static LED_GPIO_T Gpio_List[] = {
     {LED_RED_GPIO_Port,LED_RED_Pin},
};


#define LED_NUM_MAX (sizeof(Gpio_List)/sizeof(Gpio_List[0]))


void Turn_LedOn(uint8_t LedNo)
{
		if(LedNo >= LED_NUM_MAX)
		{
			return;
		}
		else
		{
			HAL_GPIO_WritePin(Gpio_List[LedNo].GPIOx,Gpio_List[LedNo].GPIO_Pin,GPIO_PIN_SET);
		}
}

void Turn_OffLed(uint8_t LedOff)
{
		if(LedOff >= LED_NUM_MAX){
			return;
		}
		else
		{
			HAL_GPIO_WritePin(Gpio_List[LedOff].GPIOx,Gpio_List[LedOff].GPIO_Pin,GPIO_PIN_RESET);
		}
}

void Turn_ToggleLed(uint8_t LedToggle)
{
		if(LedToggle >= LED_NUM_MAX)
		{
			return;
		}
		else
		{
			HAL_GPIO_TogglePin(Gpio_List[LedToggle].GPIOx,Gpio_List[LedToggle].GPIO_Pin);
		}
}
