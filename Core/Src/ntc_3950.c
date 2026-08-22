/*
 * ntc_3950.c
 *
 *  Created on: Sep 13, 2024
 *      Author: dongjundj, FORTUNE
 */

#include "ntc_3950.h"


//建立（温度-阻值）表
const NTC_10K_t  ntc_10k_table[]={
{125,341},{124,350},{123,359},{122,368},{121,378},
{120,388},{119,399},{118,409},{117,420},{116,432},{115,443},{114,456},{113,468},{112,481},{111,495},
{110,508},{109,523},{108,538},{107,553},{106,569},{105,585},{104,602},{103,619},{102,637},{101,656},
{100,675},{99,696},{98,716},{97,738},{96,760},{95,783},{94,807},{93,831},{92,857},{91,884},
{90,911},{89,940},{88,969},{87,1000},{86,1032},{85,1065},{84,1099},{83,1134},{82,1171},{81,1209},
{80,1249},{79,1290},{78,1333},{77,1378},{76,1424},{75,1472},{74,1522},{73,1574},{72,1628},{71,1684},
{70,1742},{69,1803},{68,1866},{67,1932},{66,2001},{65,2072},{64,2146},{63,2224},{62,2304},{61,2388},
{60,2476},{59,2567},{58,2662},{57,2761},{56,2865},{55,2973},{54,3086},{53,3203},{52,3326},{51,3454},
{50,3588},{49,3728},{48,3874},{47,4027},{46,4187},{45,4354},{44,4529},{43,4711},{42,4903},{41,5103},
{40,5313},{39,5532},{38,5762},{37,6003},{36,6255},{35,6520},{34,6797},{33,7088},{32,7393},{31,7713},
{30,8049},{29,8402},{28,8773},{27,9162},{26,9570},{25,10000},{24,10452},{23,10926},{22,11426},{21,11951},
{20,12504},{19,13086},{18,13699},{17,14345},{16,15025},{15,15742},{14,16498},{13,17294},{12,18135},{11,19022},
{10,19958},{9,20947},{8,21992},{7,23096},{6,24263},{5,25497},{4,26800},{3,28178},{2,29636},{1,31179},
{0,32814},{-1,34548},{-2,36385},{-3,38333},{-4,40400},{-5,42592},{-6,44916},{-7,47382},{-8,50002},{-9,52786},
{-10,55744},{-11,58890},{-12,62237},{-13,65797},{-14,69588},{-15,73626},{-16,77927},{-17,82513},{-18,87402},{-19,92618},
{-20,98185},{-21,104130},{-22,110480},{-23,117266},{-24,124522},{-25,132284},{-26,140571},{-27,149441},{-28,158941},{-29,169120},
{-30,180032},{-31,191735},{-32,204292},{-33,217774},{-34,232254},{-35,247816},{-36,264549},{-37,282549},{-38,301925},{-39,322791},
{-40,345275},
};

NTC_OBJ_t NTC_OBJ={0};

#define FILTER_SMA_ORDER 12
uint16_t Filter_Buffer[FILTER_SMA_ORDER] = { 0 };

uint16_t ADC_Raw[1]={0};
/**
 * @brief 滤波算法
 */
uint16_t Filter_SMA(uint16_t For_Filtered)
{
    static  uint8_t  cnt_filter=1;
    uint32_t Output = 0;  	//返回值

    Filter_Buffer[FILTER_SMA_ORDER - 1] = For_Filtered; // 取一个新值
    for (uint8_t i = 0; i < FILTER_SMA_ORDER; i++)
    {
        Output += Filter_Buffer[i];
    }

    if(cnt_filter <= FILTER_SMA_ORDER)
    {
    	Output /= cnt_filter;
    	cnt_filter++;
    }
    else
    {
        Output /= FILTER_SMA_ORDER;
    }

    for (uint8_t i = 0; i < FILTER_SMA_ORDER; i++)
    {
        Filter_Buffer[i] = Filter_Buffer[i + 1];
    }

    return (uint16_t) Output;
}

/**
 * @brief 温度计算
 */

void NTC_DataComp(NTC_OBJ_t *ntc_obj_temp,uint16_t adc)
{
	float ntc_now_resistance=0;
	uint8_t index=0;
	//判定NTC状态
	if(adc <= SHORT_CIRCUIT_THRESHOLD)			//小于15为短路
	{
		ntc_obj_temp->status = 1;
	}
	else if(adc >= OPEN_CIRCUIT_THRESHOLD)		//大于4080等于开路
	{
		ntc_obj_temp->status = 2;
	}
	else
	{
		ntc_obj_temp->status= 0;
	}

    if(ntc_obj_temp->status == 0)
	{
		ntc_now_resistance = (110000*adc)/(4095-adc);               /*计算NTC当前的阻值*/
		for(index=0; index<NTC_LIST_MAX_SIZE; index++)
		{
			if(ntc_10k_table[index].res >= ntc_now_resistance)
			{
				float   min_resistance = (float)ntc_10k_table[(index>0) ? index-1 : 0].res;/*得出下区间电阻值*/
				float   max_resistance = (float)ntc_10k_table[index].res;                  /*得出上区间电阻值*/
				float   d1 = (float)(max_resistance - min_resistance) / 10;                /*将该区间的阻值分为10等分*/
				uint16_t d2 = 10-(uint8_t)((ntc_now_resistance - min_resistance) / d1);    /*得出n份*0.1°C*/
                ntc_obj_temp->temp_c = (ntc_10k_table[index].temp*10)+ d2-NTC_TEMP_ADJUST;
                return;
			}
		}
	}
	else
	{
		ntc_obj_temp->temp_c = -999.9;
	}
}

/**
 * @brief 温度计算
 */

void NTC_Sensor(void)
{
  uint16_t ntc_cur_ad = 0;
  AD_TRIGGER_MODE_t ad_trigger_mode= AD_CONTINUE_TIMER;
  //Step1:取一次AD值
  switch(ad_trigger_mode)
  {
  	  case AD_SINGLE:
  		  HAL_ADC_Start(&hadc1);
  		  HAL_ADC_PollForConversion(&hadc1, 10);
  		  ntc_cur_ad = HAL_ADC_GetValue(&hadc1);
  		  HAL_ADC_Stop(&hadc1);
  		  break;
  	  case AD_CONTINUE_TIMER:
  		  ntc_cur_ad = HAL_ADC_GetValue(&hadc1);
  		  break;
  	  case AD_CONTINUE_TIMER_DMA:
  		ntc_cur_ad = ADC_Raw[0];
  		  break;
  }


  //Step2:滤波处理
  Filter_SMA(ntc_cur_ad);
  //Step3:计算一次温度值
  NTC_DataComp(&NTC_OBJ,ntc_cur_ad);

  printf("ntc-temp:%d\r\n",NTC_OBJ.temp_c); //日志打印

}

