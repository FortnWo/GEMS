/*
 * ntc_3950.h
 *
 *  Created on: Sep 13, 2024
 *      Author: dongjundj, FORTUNE
 */

#ifndef INC_NTC_3950_H_
#define INC_NTC_3950_H_

#include "main.h"

typedef struct
{
	int temp;     //单位：℃
	float res;    //单位：Ω
}NTC_10K_t;


typedef enum
{
	NTC_NORMAL=0,
	NTC_SHORTCIRCUIT,
	NTC_OPENCIRCUIT
}NTC_STATUS_TYPE;

typedef enum
{
	AD_SINGLE=1,
	AD_CONTINUE_TIMER,
	AD_CONTINUE_TIMER_DMA,
}AD_TRIGGER_MODE_t;

typedef struct
{
	int16_t temp_c;
	int16_t temp_f;
	NTC_STATUS_TYPE status;
}NTC_OBJ_t;


extern uint16_t ADC_Raw[1];
extern NTC_OBJ_t NTC_OBJ;

#define SHORT_CIRCUIT_THRESHOLD 15
#define OPEN_CIRCUIT_THRESHOLD  4080

#define NTC_LIST_MAX_SIZE       (sizeof(ntc_10k_table) / sizeof(ntc_10k_table[0]))/*表的长度*/
#define ADC_REFERENCE_VOLTAGE   (float)3300                                       /*adc 电压基准*/
#define NTC_TEMP_ADJUST         (uint16_t)50

void NTC_DataComp(NTC_OBJ_t *ntc_obj_temp,uint16_t adc);
void NTC_Sensor(void);

#endif /* INC_NTC_3950_H_ */
