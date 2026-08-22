/*
 * timeslice.h
 *
 *  Created on: Sep 5, 2024
 *      Author: dongjundj, FORTUNE
 */

#ifndef INC_TIMESLICE_H_
#define INC_TIMESLICE_H_


#include  "main.h"


extern __IO uint64_t timeslice_uwTick;  //计数值

void TimeSlice_IncTick(void);
uint64_t TimeSlice__GetTick(void);




#endif /* INC_TIMESLICE_H_ */
