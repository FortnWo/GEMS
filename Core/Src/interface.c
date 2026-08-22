/*
 * interface.c
 *
 *  Created on: Oct 11, 2024
 *      Author: dongjundj, FORTUNE
 */

#include "interface.h"

uint8_t page_id =1;

void StartupInterface(void)
{
	  OLED_NewFrame();
	  OLED_DrawImage((128 - (logo1Img.w)) / 2, 0, &logo1Img, OLED_COLOR_NORMAL);
	  OLED_PrintString(10, 64 - 24, "农业大棚PRO", &font16x16, OLED_COLOR_NORMAL);
	  OLED_ShowFrame();
}




void Page_Temp(void)
{
	  char data[30];
	  float  f_temp;
	  f_temp = AHT10_Dev.temp_c/10;
	  OLED_NewFrame();
	  OLED_DrawImage(0, 0, &temp_logoImg, OLED_COLOR_NORMAL);
	  OLED_PrintString(110,64-16, "℃", &font16x16, OLED_COLOR_NORMAL);
	  sprintf(data,"%03.1f",f_temp);
	  OLED_PrintString(60,10, data, &font16x16, OLED_COLOR_NORMAL);
	  OLED_ShowFrame();
}


void Page_Humi(void)
{
	char data[30];
	OLED_NewFrame();
	OLED_DrawImage(0, 0, &humi_logoImg, OLED_COLOR_NORMAL);
	OLED_PrintASCIIString(100, 64-20,"%RH", &afont16x8, OLED_COLOR_NORMAL);
	sprintf(data,"%03d",AHT10_Dev.humi);
	OLED_PrintString(60,10, data, &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();
}


void Page_Light(void)
{
	char data[30];
	OLED_NewFrame();
	OLED_DrawImage(0,15, &sImg, OLED_COLOR_NORMAL);
	OLED_PrintASCIIString(100, 64-20,"LUX", &afont16x8, OLED_COLOR_NORMAL);

	sprintf(data,"%05d",BH1750_Dev.light);
	OLED_PrintString(60,10, data, &font16x16, OLED_COLOR_NORMAL);

	OLED_ShowFrame();
}

void Page_Show(void)
{
  switch(page_id)
  {
	  case 1:
		  Page_Temp();
		  page_id++;
		  break;
	  case 2:
		  Page_Humi();
		  page_id++;
		  break;
	  case 3:
		  Page_Light();
		  page_id=1;
		  break;
  }
}
