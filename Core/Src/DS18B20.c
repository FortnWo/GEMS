#include "DS18B20.h"

static void DS18B20_IO_Mode(uint8_t in_or_out);
static uint8_t DS18B20_OneWire_Init(void);
static void DS18B20_Write_Byte(uint8_t Wdata);
static uint8_t DS18B20_Read_Byte(void);


DS18B20_Dev_t DS18B20_Dev = {0};


/**
 * Brief:  总线管脚工作方式设置
 */
static void DS18B20_IO_Mode(PinMode Mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DS18B20_IO_Pin;

    if (Mode == Input)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    }
    else if (Mode == Output)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    }
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DS18B20_IO_GPIO_Port, &GPIO_InitStruct);
}




/**
 * Brief:  DS18B20初始化：复位脉冲+存在脉冲(所有和DS18B20间的通信都以初始化序列开始)
 * Retval: 0:握手成功    1:握手失败
 */
static uint8_t DS18B20_OneWire_Init(void)
{
    uint8_t DS_flag;

    DS18B20_IO_Mode(Output);   //复位脉冲
    DS18B20_IO_Write_0();
    delay_us(600);

    DS18B20_IO_Write_1();   //释放总线
    delay_us(30);

    DS18B20_IO_Mode(Input);    //检测存在脉冲
    DS_flag = DS18B20_IO_Read();
    delay_us(500);   //500
    return DS_flag;
}


/**
 * Brief:  写操作
 */
static void DS18B20_Write_Byte(uint8_t Wdata)
{
    uint8_t i;
    for (i = 8; i > 0; i--)
    {
        DS18B20_IO_Mode(Output);     //前导脉冲
        DS18B20_IO_Write_0();
        delay_us(2);              //T>1us

        if ((Wdata & 0x01) == 1)  //从最低位开始写
            DS18B20_IO_Write_1();
        else
            DS18B20_IO_Write_0();

        delay_us(70);            //60us < 数据位写 <120us

        DS18B20_IO_Write_1();
        Wdata >>= 1;
    }
}

/**
 * Brief:  读操作
 */
static uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i;
    uint8_t Rdata = 0;
    for (i = 8; i > 0; i--)
    {
        DS18B20_IO_Mode(Output);     //前导脉冲
        DS18B20_IO_Write_0();
        delay_us(2);

        DS18B20_IO_Write_1();    //释放总线
        delay_us(15);

        Rdata >>= 1;
        DS18B20_IO_Mode(Input);    //DS18B20低位先传
        if (DS18B20_IO_Read() == 1)
            Rdata |= 0x80;

        delay_us(50);
    }
    return Rdata;
}

/**
 * Brief:  设备初始化
 */
void DS18B20_Init(void)
{
  uint8_t i=0;

  for(i=0;i<20;i++)
  {
	  if(DS18B20_OneWire_Init()==0)
	  {
		  DS18B20_Dev.status = DS18B20_NORMAL;
		  DS18B20_OneWire_Init();
		  DS18B20_Write_Byte(0xcc); //跳过读取序列号(ROM)
		  DS18B20_Write_Byte(0x44); //启动温度检测
		  printf("DS18B20 is OK!\r\n");
	  	  break;
	  }
	  delay_ms(1);
  }

  if(i>=20)
  {
	  printf("DS18B20 can't be found!\r\n");
	  DS18B20_Dev.status = DS18B20_SHORTCIRCUIT;
  }
}


/**
 * Brief:  获取温度值
 */
void DS18B20_GetTemperature(void)
{
    /*
    读到的温度数据 Byte0,Byte1 (12位精度)
    TempL  2^3  2^2  2^1  2^0  2^-1  2^-2  2^-3  2^-4
    TempH   S    S    S    S    S    2^6   2^5   2^4
    S=0(温度为正)  S=1(温度为负)
    */
	uint16_t temperature;
	uint8_t TempL;
    uint8_t TempH;
    int8_t minus = 0;   //0:正   1:负

    DS18B20_OneWire_Init();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe); //读取温度寄存器内容(读取头两个值为温度)

    TempL = DS18B20_Read_Byte(); //读取Byte0温度数据
    TempH = DS18B20_Read_Byte(); //读取Byte1温度数据

    temperature = (TempH << 8) | TempL;

	if (temperature & 0x8000)    //负温度时存储的是补码--取反+1
	{
			temperature = ~temperature + 1;
			minus = 1;
	}

	if (minus)
		DS18B20_Dev.temp_c = (int16_t)(((double)temperature*0.0625*10+0.5)*(-1.0));
	else
		DS18B20_Dev.temp_c = (int16_t)((double)temperature*0.0625*10+0.5);

	DS18B20_OneWire_Init();   //启动下一次温度采集
	DS18B20_Write_Byte(0xcc); //跳过读取序列号(ROM)
	DS18B20_Write_Byte(0x44); //启动温度检测

}



void DS18B20_Test(void)
{
	if(DS18B20_Dev.status == DS18B20_SHORTCIRCUIT)   return;
	DS18B20_GetTemperature(); //获取温度
	printf("DS18B20-Temp:%d\r\n", DS18B20_Dev.temp_c);

}
