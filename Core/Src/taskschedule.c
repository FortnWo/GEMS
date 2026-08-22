
#include "taskschedule.h"


void LedTask(void)
{
	//HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	static uint8_t cnt =0;
	static uint8_t f_updown = 1;


		cnt++;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, cnt);
		if(cnt == 100)
		{
			cnt = 0;
		}

}

void Data_Sensor(void)
{
	NTC_Sensor();
	DS18B20_Test();
	AHT10_Test();
	BH1750_test();
}
/*
   *****************************************
   * @brief : 结构体数组
   * @param : 存放结构体变量值
   * @retval:
   *****************************************
*/

static void test()
{
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin); // 短按切换LED灯状态
	HAL_Delay(10000);
}


static TaskComps_t g_taskComps[] =
{
	{0, 50, 50, LedTask},
	{0, 1000, 1000, Data_Sensor},
	{0, 3400, 3400, Page_Show},
	{0,10000,10000,DataUpLoad},
	{0, 1000, 1000, ExceptionAlarm},
	{0, 1800000, 1800000, ExceptionHandle}
};


// 求取结构体数组大小
#define TASK_NUM_MAX (sizeof(g_taskComps) / sizeof(g_taskComps[0]))




/*
   *****************************************
   * @brief : TaskHandle任务句柄
   * @param : 无参数
   * @param : 无参数
   * @retval: 无返回值
   *****************************************
*/
void TaskHandle(void)
{
	uint8_t i = 0;
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		if(g_taskComps[i].run)
		{
			// 调度标志重新设置为0
			g_taskComps[i].run = 0;
			// 执行任务功能模块
			g_taskComps[i].pTaskFunCb();
		}
	}

}

/*
   *****************************************
   * @brief : 定时器中断服务函数中间接调用设置时间片标志
			  需要定时器1秒产生一次中断
   * @param : 无参数
   * @param : 无参数
   * @retval: 无返回值
   *****************************************
*/
void TaskScheduleCb(void)
{
	uint8_t i = 0;
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		if(g_taskComps[i].timCount)
		{
			g_taskComps[i].timCount--;
			if(g_taskComps[i].timCount == 0)
			{
				g_taskComps[i].run = 1;
				g_taskComps[i].timCount = g_taskComps[i].timRload;
			}
		}

	}

}


