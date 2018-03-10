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
	Delay_Init();				//��ʱ������ʼ��
	HAL_LED_Init();				//LED�Ƴ�ʼ��
	HAL_ELECTRIC_Init();		//AD�ɼ���ʼ��
	HAL_KEYBOARD_Init();		//������ʼ��
	HAL_Timer_TIM4Init();		//��ʼ������ʱ����ʼ��
	HAL_AllSwitch_Init();		//AD�ɼ����س�ʼ�����Լ��ܿ��س�ʼ��
	HAL_UART_Init();			//���ڳ�ʼ��
	
	FLASH_Unlock();				//flash����
	
	MID_KEYBOARD_KeyPressInit();	//��ʼ����������ʼ��
//	g_u32Battery3VFlag = 1;
	while(1)
	{
		MID_KEYBOARD_TASK();		//��������
		
		APP_MAIN_Task();			//������ɨ��
	}
	
}




