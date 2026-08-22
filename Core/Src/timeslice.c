/*
 * timeslice.c
 *
 *  Created on: Sep 5, 2024
 *      Author: dongjundj, FORTUNE
 */

#include  "timeslice.h"


__IO uint64_t timeslice_uwTick=0;  //计数值




void TimeSlice_IncTick(void)
{
	timeslice_uwTick++;
}



uint64_t TimeSlice__GetTick(void)
{
  return timeslice_uwTick;
}

