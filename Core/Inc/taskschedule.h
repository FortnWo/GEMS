#ifndef __TASKSCHEDULE_H__
#define __TASKSCHEDULE_H__

#include "main.h"

/*
   *****************************************
   * @brief : 任务组件TaskComps_t
   * @param : run 任务调度标志 1 ：调度， 0 ： 挂起
   * @param : timCount 时间片计数值
   * @param : timRload 时间片重载值
   * @param : void (*pTaskFunCb)(void) 函数指针变量，存储业务功能模块的函数地址
   * @retval: 无
   *****************************************
*/
typedef struct
{
	uint8_t  run;
	uint32_t timCount;
	uint32_t timRload;
	void (*pTaskFunCb)(void);
}TaskComps_t;




void TaskHandle(void);
void TaskScheduleCb(void);




#endif /*__ TASKSCHEDULE_H__ */
