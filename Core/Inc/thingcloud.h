/*
 * thingcloud.h
 *
 *  Created on: Oct 15, 2024
 *      Author: dongjundj, FORTUNE
 */

#ifndef INC_THINGCLOUD_H_
#define INC_THINGCLOUD_H_

#include "main.h"
#include "usart.h"

#define BOOL_TO_STRING(x) ((x) ? "true" : "false")

#define DMA_REC_SIZE   1024      // DMA缓存大小
#define USER_BUFF_SIZE 512	     // 用户数据缓存大小
#define AT_BUFF_SIZE   512		 // AT指令缓存大小


typedef struct
{
	uint8_t  UserRecFlag;  		 //用户数据接收到标志位
	uint8_t  AtRecFlag;  		 //AT数据接收到标志位
	uint16_t DMARecLen; 		 //DMA接受长度
	uint16_t UserRecLen;   		 //用户未处理数据长度
	uint16_t AtRecLen;    		 //AT未处理数据长度
	uint8_t  DMARecBuffer[DMA_REC_SIZE];  //DMAbuffer
	uint8_t  UserBuffer[USER_BUFF_SIZE];  // 用户数据处理buffer
	uint8_t  AtBuffer[AT_BUFF_SIZE];	  // AT指令数据处理buffer
}UartDataType_Wifi;

extern UartDataType_Wifi WifiData;

extern DMA_HandleTypeDef hdma_lpuart_rx;



void WifiAPPDataHandle(void);

void ThingCloud_Conn(void);

void DataUpLoad(void);



#endif /* INC_THINGCLOUD_H_ */
