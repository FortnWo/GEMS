/*
 * Beep.c
 *
 *  Created on: Dec 18, 2024
 *      Author: FORTUNE
 */

#include "Beep.h"

// 一次BEEP启停
void Beep(void)
{
	HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
	HAL_Delay(100);
	HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
}

void Beeping()
{
	HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
}
