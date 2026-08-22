/*
 * key.h
 *
 *  Created on: Sep 5, 2024
 *      Author: dongjundj, FORTUNE
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_

#include "main.h"

#define KEY_SHORT_PRESS   0x01
#define KEY_LONG_PRESS    0x41
#define KEY_DOUBLE_PRESS  0x02
#define KEY_TRIPLE_PRESS  0x03


/*
   定义枚举类型的数据表示状态机中4种不同的状态
*/
typedef enum
{
    KEY_RELEASE = 0,    	//按键松开的状态
	KEY_CONFIRM,            //按键消抖的状态
	KEY_SHORTPRESS,         //按键短按的状态
	KEY_LONGPRESS	        //按键长按的状态

}KEY_STATE;


/*
   定义结构体数据类型表示每一个按键的状态
*/
typedef struct
{
	KEY_STATE keyState;    //按键对象当前的状态
	uint64_t  preSysTime;  //按键对象一次按键事件的起始的状态
}key_info_t;



#define  CONFIRM_TIME  10 	// 按键消除抖动时间窗10ms
#define  LONGPRESS_TIME 2000 // 按键长时间按下时间窗
#define  COUNT_TIME  1250


static uint16_t keyCount = 0;

void KeyHandle(void);


#endif /* INC_KEY_H_ */
