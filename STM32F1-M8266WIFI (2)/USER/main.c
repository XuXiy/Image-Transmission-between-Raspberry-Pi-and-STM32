/********************************************************************
 * main.c
 * .Description
 *     Source file of main entry 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "misc.h"
#include "M8266HostIf.h"
#include "led.h"
#include "key.h"
#include "M8266WIFIDrv.h"
#include "M8266WIFI_ops.h"
#include "brd_cfg.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"

extern u8 RecvData[];
void M8266WIFI_Test(void);

int main(void)
{ 
	u8 success=0;
	u8 i;
	//u8 j;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//	Setup Interrupt Priority Group2
	delay_init();
	OLED_Init();
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Clear();

#ifdef USE_LED_AND_KEY_FOR_TEST
  LED_Init();																			// Initialise LEDs
  KEY_Init();																			// Initialise KEYs
#endif
	
  /////////////////////////////////////////////////////////////////////////////////////////////////////
	//1. Call M8266HostIf_Init() to initialize the MCU Host Interface����M8266HostIf_Init()����ʼ��MCU�����ӿ�
	//   - including SPI and nREST/nCS GPIO pins����SPI��nREST/nCS GPIO����
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	M8266HostIf_Init();
	
#ifdef USE_LED_AND_KEY_FOR_TEST	
  
  //Flash 5 times both of the two LEDs simutaneously indicating that next will initialisation the M8266WIFI module
	for(i=0; i<5; i++)
  {
		LED_set(0, 0); LED_set(1, 0);  M8266WIFI_Module_delay_ms(50);
		LED_set(0, 1); LED_set(1, 1);  M8266WIFI_Module_delay_ms(50);
	}
#endif
	
  /////////////////////////////////////////////////////////////////////////////////////////////////////
	//2. Call M8266WIFI_Module_Init_Via_SPI() to initialize the wifi module via SPI Interface����M8266WIFI_Module_Init_Via_SPI()ͨ��SPI�ӿڳ�ʼ��wifiģ��
  //   - Including: Module reset, module select, module connecting wifi if required, and etc	����:ģ�鸴λ��ģ��ѡ�񡢱�Ҫʱģ������wifi��
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	success = M8266WIFI_Module_Init_Via_SPI();
  if(success)
	{
		
#ifdef USE_LED_AND_KEY_FOR_TEST			
	    for(i=0; i<3; i++)  // Flash 3 times the Two LEDs alternatively in the Main Board indicating M8266WIFI initialised successfully�������Ͻ�����˸2��led 3�Σ���ʾM8266WIFI��ʼ���ɹ�
	    {
	       LED_set(0, 1); LED_set(1, 0);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 0); LED_set(1, 1);  M8266WIFI_Module_delay_ms(100);
			}
		  LED_set(0, 1); LED_set(1, 1);
#endif
	}	
	else // If M8266WIFI module initialisation failed, two led constantly flash in 2Hz
	{
		  while(1)
			{
#ifdef USE_LED_AND_KEY_FOR_TEST					
	       LED_set(0, 1); LED_set(1, 1);  M8266WIFI_Module_delay_ms(2000);
		     LED_set(0, 0); LED_set(1, 0);  M8266WIFI_Module_delay_ms(2000);
#endif
			}
	}

	M8266WIFI_Test(); //

	while(1)
	{

	//LCD_ShowPicture(RecvData);
		

	}
} //end of main 

