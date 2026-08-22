/*
 * key.c
 *
 *  Created on: Sep 5, 2024
 *      Author: dongjundj, FORTUNE
 */

#include "key.h"

static key_info_t g_key = {0};

//定义一个变量记录按键次数

static uint8_t KeyScan(void)
{
	/*
	   使用switch case 的方式实现状态机
	*/

	uint8_t keypress; // 按键管脚电平

	// 定义一个变量记录当前系统时间
	uint64_t curSysTime;
	/*
	  判断GPIO口的状态，如果引脚电平变为0表示按键被按下
	*/
	keypress = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);


	switch (g_key.keyState)
	{
	case KEY_RELEASE:
		// 判断按键是否按下，按下就切换为消除抖动的状态
		if (!keypress)
		{
			// 切换按键的状态

			g_key.keyState = KEY_CONFIRM;
			// 按键按下获取系统运行时间
			g_key.preSysTime = TimeSlice__GetTick();
		}
		break;
	case KEY_CONFIRM:
		/*
			 判断按键是否处于按下的状态
		*/
		if (!keypress)
		{
			curSysTime = TimeSlice__GetTick();
			if (curSysTime - g_key.preSysTime > CONFIRM_TIME)
			{
				g_key.keyState = KEY_SHORTPRESS;
			}
		}
		else
		{
			g_key.keyState = KEY_RELEASE;
		}
		break;
	case KEY_SHORTPRESS:
		// 判断按键是否松开，松开的情况下按键是高电平
		if (keypress)
		{

			g_key.keyState = KEY_RELEASE;
			// 返回键码值,短按对应0x01，0x02，0x03
			return 0x01;
		}
		else
		{
			curSysTime = TimeSlice__GetTick();
			if (curSysTime - g_key.preSysTime > LONGPRESS_TIME)
			{
				g_key.keyState = KEY_LONGPRESS;
			}
		}
		break;
	case KEY_LONGPRESS:
		if (keypress)
		{
			g_key.keyState = KEY_RELEASE;
			return 0x41;
		}
		break;
	default:
		g_key.keyState = KEY_RELEASE;
		break;
	}
	// 不是在上面返回的码值，就返回0
	return 0;
}

uint8_t GetVal(void)
{
	uint8_t res = 0;
	res = KeyScan();
	return res;
}

void ledRed()
{
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin); // 短按切换LED灯状态
	HAL_Delay(100);
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin); // 短按切换LED灯状态
}

void KeyJudge(void)
{
	if(keyCount)
	{
		switch(keyCount)
		{
		case 1:
			Beep();

			keyCount = 0;
			break;
		case 2:
			Beep();
			HAL_Delay(100);
			Beep();

			keyCount = 0;
			break;
		case 3:
			Beep();
			HAL_Delay(100);
			Beep();
			HAL_Delay(100);
			Beep();

			keyCount = 0;
			break;
		default:

			keyCount = 0;
			break;
		}
	}
	else return;
}

void KeyHandle(void)
{
	static uint64_t preTime = 0, durTime = 0;
	uint8_t keyvalue = 0;
	keyvalue = KeyScan();

	switch (keyvalue)
	{
	case 0x01:

		ledRed();			//短按红灯闪烁

		keyCount++;

		if(!preTime) preTime = TimeSlice__GetTick();

		if(keyCount > 3)	//检测非法按键次数（多于3次）
		{
			preTime = 0;
			durTime = 0;
			keyCount = 0;
		}

		break;
	case 0x41:
		// HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);		//长按切换BEEP状态

		keyCount = 0;

		if (!totalAlarmState)
		{
			Beep();
			HAL_Delay(100);
			Beep();
			HAL_Delay(100);
			Beep();
			HAL_Delay(100);
			Beep();

			totalAlarmState = 1;
		}
		else
		{
			Beep();
			totalAlarmState = 0;
		}

		break;

	default:
		break;
	}

	//判断短按情况
	if(preTime)
	{
		durTime = TimeSlice__GetTick() - preTime;
		if(durTime >= COUNT_TIME)
		{
			KeyJudge();
			preTime = 0;
			durTime = 0;
		}
	}
}


