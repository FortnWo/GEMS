/*
 * thingcloud.c
 *
 *  Created on: Oct 15, 2024
 *      Author: dongjundj, FORTUNE
 */

#include "thingcloud.h"

UartDataType_Wifi WifiData = {0};

/*ThingsCloud云平台Username和Password*/
 char userName[]="sxogjo1rkdhgc54j";
 char userPwd[]="teYOgi0rcZ";

void WIFI_RxCallback()
{
	// printf("%s\r\n",WifiData.DMARecBuffer);
	// memset(WifiData.DMARecBuffer, 0, DMA_REC_SIZE);
	char *addr = strstr((char *)WifiData.DMARecBuffer, "+MQTTSUBRECV");
	const char s[2] = ",";
	char *token;
	if (addr) // 若数据来自于云平台
	{
		// 目前仅处理两类数据
		//+MQTTSUBRECV:0,"attributes/push",27,{"ventilator_status":false}
		//+MQTTSUBRECV:0,"command/send/1000",52,{"method":"ventilator","params":{"cmd":1},"id":1000}
		//{"sprinkler_status":false}

		/* 获取第一个子字符串 */
		// token = strtok(addr, s);//第1个串
		// token = strtok(NULL, s);//第2个串
		// token = strtok(NULL, s);//第3个串
		// token = strtok(NULL, s);//第4个串
		token = strstr(addr, "{");

		if (WifiData.UserRecLen > 0) // 判断云平台数据缓冲区是否有未处理数据,若有则将新的数据添加在后面；
		{
			memcpy(&WifiData.UserBuffer[WifiData.UserRecLen], token, strlen(token)); // 转存到用户数据待处理区域
			WifiData.UserRecLen += strlen(token);
		}
		else // 向云平台数据缓冲区直接添加新数据
		{
			memcpy(WifiData.UserBuffer, token, strlen(token)); // 转存到用户数据待处理区域
			WifiData.UserRecLen = strlen(token);
		}
		WifiData.UserRecFlag = 1;
	}
	else // 若数据为AT指令回显数据
	{
		if (WifiData.AtRecLen > 0) // 判断AT指令数据数组中是否有未处理数据，若有则将新的数据添加在后面；
		{
			memcpy(&WifiData.AtBuffer[WifiData.AtRecLen], WifiData.DMARecBuffer, WifiData.DMARecLen); // 转存到AT指令数据待处理区域
			WifiData.AtRecLen += WifiData.DMARecLen;
		}
		else // 向AT指令数据缓冲区直接添加新数据
		{
			memcpy(WifiData.AtBuffer, WifiData.DMARecBuffer, WifiData.DMARecLen); // 转存到AT指令数据待处理区域
			WifiData.AtRecLen = WifiData.DMARecLen;
		}
		WifiData.AtRecFlag = 1;
	}
	memset(WifiData.DMARecBuffer, 0x00, DMA_REC_SIZE);
}

// 用户自定义中断处理函数--【----_it.h对应串口中断中调用void USARTx_IRQHandler(void)】
void WIFI_UART_Handler(void)
{
	if (__HAL_UART_GET_FLAG(&hlpuart1, UART_FLAG_IDLE) == SET) // 触发空闲中断
	{
		uint16_t temp = 0;
		__HAL_UART_CLEAR_IDLEFLAG(&hlpuart1); // 清除串口1空闲中断标志位
		HAL_UART_DMAStop(&hlpuart1);		  // 关闭DMA
		temp = __HAL_DMA_GET_COUNTER(&hdma_lpuart_rx);
		WifiData.DMARecLen = DMA_REC_SIZE - temp;							  // 总计数减去未传输的数据个数，得到已经接收的数据个数
		WIFI_RxCallback();													  // 串口接收回调函数
		HAL_UART_Receive_DMA(&hlpuart1, WifiData.DMARecBuffer, DMA_REC_SIZE); // 重新打开DMA接收
	}
}

void WifiAPPDataHandle(void)
{
	if (WifiData.AtRecFlag == 1)
	{
		printf("%s\r\n", WifiData.AtBuffer);
		memset(WifiData.AtBuffer, 0, AT_BUFF_SIZE);
		WifiData.AtRecLen = 0;
		WifiData.AtRecFlag = 0;
	}

	if (WifiData.UserRecFlag == 1)
	{
		//设备状态下发
		if (strstr(WifiData.UserBuffer, "{\"sprinkler_state\":false}"))
			HAL_GPIO_WritePin(Dev_Ctrl1_GPIO_Port, Dev_Ctrl1_Pin, GPIO_PIN_RESET);
			sprinkler = false;

		if (strstr(WifiData.UserBuffer, "{\"sprinkler_state\":true}"))
			HAL_GPIO_WritePin(Dev_Ctrl1_GPIO_Port, Dev_Ctrl1_Pin, GPIO_PIN_SET);
			sprinkler = true;

		if (strstr(WifiData.UserBuffer, "{\"ventilator_state\":false}")) //{"ventilator_status":true}
			HAL_GPIO_WritePin(Dev_Ctrl1A15_GPIO_Port, Dev_Ctrl1A15_Pin, GPIO_PIN_RESET);
			ventilator = false;

		if (strstr(WifiData.UserBuffer, "{\"ventilator_state\":true}"))
			HAL_GPIO_WritePin(Dev_Ctrl1A15_GPIO_Port, Dev_Ctrl1A15_Pin, GPIO_PIN_SET);
			ventilator = false;

		if (strstr(WifiData.UserBuffer, "{\"visor_state\":true}"))
			visor = true;

		if (strstr(WifiData.UserBuffer, "{\"visor_state\":false}"))
			visor = false;

		if (strstr(WifiData.UserBuffer, "{\"light_state\":true}"))
			light = true;

		if (strstr(WifiData.UserBuffer, "{\"light_state\":false}"))
			light = false;

		if (strstr(WifiData.UserBuffer, "{\"heater_state\":true}"))
			heater = true;

		if (strstr(WifiData.UserBuffer, "{\"heater_state\":false}"))
			heater = false;

		//警报状态下发
		if (strstr(WifiData.UserBuffer, "{\"temp_alarm\":true}"))
			singleAlarmState.temp = open;

		if (strstr(WifiData.UserBuffer, "{\"temp_alarm\":false}"))
			singleAlarmState.temp = close;

		if (strstr(WifiData.UserBuffer, "{\"humi_alarm\":true}"))
			singleAlarmState.humi = open;

		if (strstr(WifiData.UserBuffer, "{\"humi_alarm\":false}"))
			singleAlarmState.humi = close;

		if (strstr(WifiData.UserBuffer, "{\"lux_alarm\":true}"))
			singleAlarmState.lux = open;

		if (strstr(WifiData.UserBuffer, "{\"lux_alarm\":false}"))
			singleAlarmState.lux = close;

		if (strstr(WifiData.UserBuffer, "{\"soil_temp_alarm\":true}"))
			singleAlarmState.soil_temp = open;

		if (strstr(WifiData.UserBuffer, "{\"soil_temp_alarm\":false}"))
			singleAlarmState.soil_temp = close;

		if (strstr(WifiData.UserBuffer, "{\"soil_humi_alarm\":true}"))
			singleAlarmState.soil_humi = open;

		if (strstr(WifiData.UserBuffer, "{\"soil_humi_alarm\":false}"))
			singleAlarmState.soil_humi = close;

		if (strstr(WifiData.UserBuffer, "{\"co2_alarm\":true}"))
			singleAlarmState.co2 = open;

		if (strstr(WifiData.UserBuffer, "{\"co2_alarm\":false}"))
			singleAlarmState.co2 = close;


		printf("%s\r\n", WifiData.UserBuffer);
		memset(WifiData.UserBuffer, 0, USER_BUFF_SIZE);
		WifiData.UserRecLen = 0;
		WifiData.UserRecFlag = 0;
	}
}

void ThingCloud_Conn(void)
{
	uint8_t cmdMQTTCONN[] = "AT+MQTTCONN=0,\"sh-1-mqtt.iot-api.com\",1883,0\r\n";
	uint8_t cmdMQTTSUBREVCMD[] = "AT+MQTTSUB=0,\"command/send/1000\",1\r\n";
	uint8_t cmdMQTTSUBREVATT[] = "AT+MQTTSUB=0,\"attributes/push\",1\r\n";

	char cmdData[100];
	sprintf(cmdData, "AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n", userName, userPwd);

	HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmdData, strlen(cmdData), 0xFFFF);
	HAL_Delay(1000);
	HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmdMQTTCONN, strlen(cmdMQTTCONN), 0xFFFF);
	HAL_Delay(1000);
	HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmdMQTTSUBREVATT, strlen(cmdMQTTSUBREVATT), 0xFFFF);
	HAL_Delay(1000);
}

void DataUpLoad(void)
{
	float t_temp;
	float soil_temp;

	//	t_temp = AHT10_Dev.temp_c/10;
	t_temp = NTC_OBJ.temp_c / 10;
	soil_temp = DS18B20_Dev.temp_c / 10;

	char cmdData[1024];
	sprintf(cmdData, "AT+MQTTPUB=0,\"attributes\",\"{\\\"temperature\\\":%.1f\\,\\\"humidity\\\":%d\\,\\\"luminosity\\\":%d\\,\\\"soil_temp\\\":%.1f\\,\\\"soil_humi\\\":%d\\,\\\"co2\\\":%d\\,\\\"ventilator_state\\\":%s\\,\\\"sprinkler_state\\\":%s\\,\\\"visor_state\\\":%s\\,\\\"light_state\\\":%s\\,\\\"heater_state\\\":%s}\",0,0\r\n", t_temp, AHT10_Dev.humi, BH1750_Dev.light, soil_temp, 60, 501, BOOL_TO_STRING(ventilator), BOOL_TO_STRING(sprinkler), BOOL_TO_STRING(visor), BOOL_TO_STRING(light), BOOL_TO_STRING(heater));
	HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmdData, strlen(cmdData), 0xFFFF);
}


