/*
 * controller.h
 *
 *  Created on: Dec 18, 2024
 *      Author: FORTUNE
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "main.h"

//最小状态检查时间(min)
#define CHECK_TIME_MIN 30
#define MS_TO_MIN(x) (x/60000)

typedef enum
{
	false = 0,
	true
}bool;

typedef enum
{
	NORMAL = 0,
	HIGH,
	LOW
} AttributeState;


//警报数值下限
typedef struct
{
	float temp, humi, lux, soilTemp, soilHumi, co2;
} Low;

//警报数值上限
typedef struct
{
	float temp, humi, lux, soilTemp, soilHumi, co2;
}High;

typedef struct
{
	AttributeState state;
}AttributeType;

typedef struct
{
	AttributeType temp;
	AttributeType humi;
	AttributeType lux;
	AttributeType soil_temp;
	AttributeType soil_humi;
	AttributeType co2;
}ExceptionsSet;

void ExceptionHandle();

extern bool ventilator;
extern bool sprinkler;
extern bool visor;
extern bool light;
extern bool heater;

#endif /* INC_CONTROLLER_H_ */
