#include "delay.h"


/**
* @Brief 毫秒延时函数
*/

void delay_ms(uint16_t ms)
{
   HAL_Delay(ms);
}

void DelayMinutes(uint16_t min)
{
	HAL_Delay(60000 * min);
}

/**
* @Brief 微秒延时函数
*/
void delay_us(uint16_t us)
{
	uint16_t differ = 0xffff-us-5;				
	__HAL_TIM_SET_COUNTER(&htim7,differ);	      ///设定TIM7计数器起始值ֵ
	HAL_TIM_Base_Start(&htim7);		              //启动定时器
	
	while(differ < 0xffff-5)
	{	//判断
		differ = __HAL_TIM_GET_COUNTER(&htim7);		//查询计数器的计数值ֵ
	}
	HAL_TIM_Base_Stop(&htim7);                      //关闭定时器
}
