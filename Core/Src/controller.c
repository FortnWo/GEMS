/*
 * controller.c
 *
 *  Created on: Dec 18, 2024
 *      Author: FORTUNE
 */

#include "controller.h"


//自动化设备状态
bool ventilator = 0, sprinkler = 0, visor = 0, light = 0, heater = 0;

//异常标志位
ExceptionsSet exceptions;

//异常标志位
uint8_t e = 0;

//安全阈值
Low lowLimit = {20.0, 30.0, 100.0, 20.0, 40.0, 300.0};
High highLimit = {35.0, 80.0, 500.0, 35.0, 80.0, 700.0};

//状态持续时间
uint64_t durTime = 0;
//异常时间
uint64_t exceptionTime = 0;


void ExceptionScan()
{
	float e_temp = NTC_OBJ.temp_c / 10;
	float soil_temp = DS18B20_Dev.temp_c / 10;
	float soil_humi = 60, co2 = 501;

	exceptions.temp.state = e_temp > highLimit.temp?HIGH:e_temp < lowLimit.temp?LOW:NORMAL;
	exceptions.humi.state = AHT10_Dev.humi > highLimit.humi?HIGH:AHT10_Dev.humi < lowLimit.humi?LOW:NORMAL;
	exceptions.lux.state = BH1750_Dev.light > highLimit.lux?HIGH:BH1750_Dev.light < lowLimit.lux?LOW:NORMAL;
	exceptions.soil_temp.state = soil_temp > highLimit.soilTemp?HIGH:soil_temp < lowLimit.soilTemp?LOW:NORMAL;
	exceptions.soil_humi.state = soil_humi > highLimit.soilHumi?HIGH:soil_humi < lowLimit.soilHumi?LOW:NORMAL;
	exceptions.co2.state = co2 > highLimit.co2?HIGH:co2 < lowLimit.co2?LOW:NORMAL;

}

//检测是否持续处于该状态
uint8_t stateCheck()
{
	static uint64_t preTime = 0;

	if(!preTime) preTime = TimeSlice__GetTick();
	else {
		durTime = TimeSlice__GetTick() - preTime;

		if(MS_TO_MIN(durTime) >= CHECK_TIME_MIN)
		{
			preTime = 0;
			//将当前状态切换
			if(!e) e = 1;
			else e = 0;

			return 1;
		}
	}
	return 0;
}

void ExceptionHandle()
{
	while(1)
	{
		ExceptionScan();

		if(!e && (exceptions.temp.state || exceptions.humi.state || exceptions.lux.state || exceptions.soil_temp.state || exceptions.soil_humi.state || exceptions.co2.state))	break;
		else if(e && (!exceptions.temp.state || !exceptions.humi.state || !exceptions.lux.state || !exceptions.soil_temp.state || !exceptions.soil_humi.state || !exceptions.co2.state)) break;

	}


	if(stateCheck())
	{
		switch(e){
		case 1:
			static uint64_t preTime = 0;
			if(!preTime) preTime = TimeSlice__GetTick();
			else exceptionTime = TimeSlice__GetTick() - preTime;

			switch(exceptions.temp.state){
			case HIGH:
				ventilator = 1;
				heater = 0;
				break;
			case LOW:
				heater = 1;
				ventilator = 0;
				break;
			default:
				break;
			}

			switch(exceptions.humi.state){
			case HIGH:
				ventilator = 1;
				break;
			case LOW:
				sprinkler = 1;
				break;
			default:
				break;
			}

			switch(exceptions.lux.state){
			case HIGH:
				visor = 1;
				break;
			case LOW:
				light = 1;
				break;
			default:
				break;
			}

			switch(exceptions.soil_temp.state){
			case HIGH:
				visor = 1;
				ventilator = 1;
				break;
			case LOW:
				 heater = 1;
				break;
			default:
				break;
			}

			switch(exceptions.soil_humi.state){
			case HIGH:
				visor = 0;
				break;
			case LOW:
				sprinkler = 1;
				break;
			default:
				break;
			}

			switch(exceptions.co2.state){
			case HIGH:
				sprinkler = 1;
				break;
			case LOW:
				sprinkler = 1;
				break;
			default:
				break;
			}
			break;

		case 0:
			ventilator = 0; sprinkler = 0; visor = 0; light = 0; heater = 0;
			break;
		default:
			break;
		}
	}

}





