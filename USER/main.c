#include "mid_electric.h"
#include "mid_keyboard.h"
#include "hal_timer.h"
#include "hal_led.h"
#include "Delay.h"
#include "app_task.h"
#include "hal_switch.h"
#include "hal_uart.h"
#include "Global.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief 
 * 
 * @return int 
 */

int main()
{
	g_u32Battery12VFlag = 0;
	g_u32Battery3VFlag = 0;

	// g_u8StartFlag = 0;
	g_u8Start = 0;
	g_u8QueryUartFlag = 0;
	g_u8ClearTimeFlag = 0;
	g_u8ClearFlag = 0;
	Delay_Init();				//延时函数初始化
	HAL_LED_Init();				//LED灯初始化
	HAL_ELECTRIC_Init();		//AD采集初始化
	HAL_KEYBOARD_Init();		//按键初始化
	HAL_Timer_TIM4Init();		//开始按键定时器初始化
	HAL_AllSwitch_Init();		//AD采集开关初始化、以及总开关初始化
	HAL_UART_Init();			//串口初始化
	
	FLASH_Unlock();				//flash解锁
	
	MID_KEYBOARD_KeyPressInit();	//开始按键计数初始化
//	g_u32Battery3VFlag = 1;
	while(1)
	{
		MID_KEYBOARD_TASK();		//按键任务
		
		APP_MAIN_Task();			//主任务扫描
	}
	
}




