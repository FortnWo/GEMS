/*
 * alarm.h
 *
 *  Created on: Dec 18, 2024
 *      Author: FORTUNE
 */

#ifndef INC_ALARM_H_
#define INC_ALARM_H_

#include "main.h"


//大棚环境状态安全时间(min)
#define S_TIME 180

typedef enum
{
	close = 0, open
}AlarmState;

typedef struct
{
	AlarmState temp;
	AlarmState humi;
	AlarmState lux;
	AlarmState soil_temp;
	AlarmState soil_humi;
	AlarmState co2;
}AlarmsSet;

void ExceptionAlarm(void);

extern uint8_t totalAlarmState;
extern AlarmsSet singleAlarmState;

#endif /* INC_ALARM_H_ */
