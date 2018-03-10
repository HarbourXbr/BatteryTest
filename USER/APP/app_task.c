#include "app_task.h"
#include "mid_keyboard.h"
#include "mid_electric.h"
#include "hal_uart.h"
#include "hal_led.h"
#include "Global.h"
#include <stdio.h>


void APP_MAIN_Task(void)
{
	if((g_u32Battery3VFlag == 0) && (g_u32Battery12VFlag == 0))
	{
		while(MID_ELECTRIC_SelfInspection())	//AD�Լ� �ж��ⲿ��12V��ػ���3.3���
		{
			printf("\r\n......�����......\r\n");
		}
	}
	if(g_u8StartFlag == 1)
	{
		if(g_u8Start == 1)
		{	
			LED_TWINKLE();
			printf("\r\n������ʼ�ɼ�����\r\n");
			g_u8Start = 0;
		}
		printf("\r\n......�ɼ���......\r\n");
		MID_ELECTRIC_SaveVolt2Mem();
	}
	if(g_u8ClearFlag == 1)
	{
		printf("\r\n......������......\r\n");
		MID_ELECTRIC_ClearVolt2Mem();
		LED_TWINKLE();
		printf("\r\n���������ɹ�����\r\n");
		g_u8ClearFlag = 0;
	}
	if(g_u8QueryUartFlag == 1)
	{
		MID_ELECTRIC_QuaryVolt2Mem();
		g_u8QueryUartFlag = 0;
	}		
}

