#ifndef __BH1750_H
#define __BH1750_H
 
#ifdef __cplusplus
 extern "C" {
#endif
 
#include "stdio.h"
#include "main.h"
 
#define BH1750_OK                1
#define BH1750_ERROR             0
 
 
#define bsp_DelayMS             HAL_Delay
#define BH1750_SLAVE_ADDRESS    0x46         /* I2C从机地址 */
 
 /* 操作码 Opercode 定义 */
enum
{
	    BHOP_POWER_DOWN = 0x00,        /* 进入掉电模式。芯片上电后缺省就是PowerDown模式 */
	    BHOP_POWER_ON   = 0x01,        /* 上电，等待测量命令 */
	    BHOP_RESET      = 0x07,        /* 清零数据寄存器 (Power Down 模式无效) */
	    BHOP_CON_H_RES  = 0x10,        /* 连续高分辨率测量模式  （测量时间 120ms） （最大 180ms）*/
	    BHOP_CON_H_RES2 = 0x11,        /* 连续高分辨率测量模式2 （测量时间 120ms）*/
	    BHOP_CON_L_RES  = 0x13,        /* 连续低分辨率测量模式 （测量时间 16ms）*/

	    BHOP_ONE_H_RES  = 0x20,        /* 单次高分辨率测量模式 , 之后自动进入Power Down */
	    BHOP_ONE_H_RES2 = 0x21,        /* 单次高分辨率测量模式2 , 之后自动进入Power Down  */
	    BHOP_ONE_L_RES  = 0x23,        /* 单次低分辨率测量模式 , 之后自动进入Power Down  */
};
 


typedef enum
{
	BH1750_NORMAL=0,
	BH1750_SHORTCIRCUIT
}BH1750_STATUS_TYPE;

//各部分温度数据
typedef struct
{
	int16_t light;
	BH1750_STATUS_TYPE status;
} BH1750_Dev_t;

extern BH1750_Dev_t BH1750_Dev;


void BH1750_Init(void);

void BH1750_test( void );
 
#ifdef __cplusplus
}
#endif
 
#endif /*__BH1750_H */
