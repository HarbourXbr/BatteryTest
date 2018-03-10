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
		while(MID_ELECTRIC_SelfInspection())	//AD自检 判断外部是12V电池还是3.3电池
		{
			printf("\r\n......检测中......\r\n");
		}
	}
	if(g_u8StartFlag == 1)
	{
		if(g_u8Start == 1)
		{	
			LED_TWINKLE();
			printf("\r\n！！开始采集！！\r\n");
			g_u8Start = 0;
		}
		printf("\r\n......采集中......\r\n");
		MID_ELECTRIC_SaveVolt2Mem();
	}
	if(g_u8ClearFlag == 1)
	{
		printf("\r\n......擦除中......\r\n");
		MID_ELECTRIC_ClearVolt2Mem();
		LED_TWINKLE();
		printf("\r\n！！擦除成功！！\r\n");
		g_u8ClearFlag = 0;
	}
	if(g_u8QueryUartFlag == 1)
	{
		MID_ELECTRIC_QuaryVolt2Mem();
		g_u8QueryUartFlag = 0;
	}		
}

