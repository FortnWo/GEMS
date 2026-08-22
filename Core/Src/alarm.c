/*
 * alarm.c
 *
 *  Created on: Dec 11, 2024
 *      Author: FORTUNE
 */


#include "alarm.h"


//警报启停状态
uint8_t totalAlarmState = 0;

AlarmsSet singleAlarmState;

extern uint64_t exceptionTime;
extern ExceptionsSet exceptions;

uint16_t tmp = 0, hm = 1, lx = 2, sT = 3, sH = 4, c2 = 5;

void AlarmTurn()
{
}

void ExceptionAlarm(void)
{
	if(!totalAlarmState) return;

	if(MS_TO_MIN(exceptionTime) >= S_TIME){
		if(singleAlarmState.temp && exceptions.temp.state)
			Beeping();
		else if(singleAlarmState.humi && exceptions.humi.state)
			Beeping();
		else if(singleAlarmState.lux && exceptions.lux.state)
			Beeping();
		else if(singleAlarmState.soil_temp && exceptions.soil_temp.state)
			Beeping();
		else if(singleAlarmState.soil_humi && exceptions.soil_humi.state)
			Beeping();
		else if(singleAlarmState.co2 && exceptions.co2.state)
			Beeping();
	}
}
