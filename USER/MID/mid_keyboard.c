#include "mid_keyboard.h"
#include "Global.h"


uint8_t g_u8StartKeyPressCunt;

static void MID_KEYBOARD_Deal(void);

/**
 * @brief ��ʼ����������ʼ��
 * 
 * @param void 
 */
void MID_KEYBOARD_KeyPressInit(void)
{
	g_u8StartKeyPressCunt = 0;
}

/**
 * @brief ��������
 * 
 * @param void 
 */
void MID_KEYBOARD_TASK(void)
{
	MID_KEYBOARD_Deal();
}

/**
 * @brief ��ʼ����״̬��ѯ
 * 
 * @param void 
 * @return KeyId 
 */

KeyId MID_KEYBOARD_KeyState(void)	
{
	while(1)
	{
		if(HAL_KEYBOARD_ISPRESSED() == START_ID)
		{
			HAL_Timer_TIM4Start();
			if(g_u8TIM4Flg == TRUE)
			{
				g_u8TIM4Flg = FALSE;
				g_u8StartKeyPressCunt++;
				if(g_u8StartKeyPressCunt >= START_KEY_TIMEOUT)//time 3s
				{
					HAL_Timer_TIM4Stop();
					g_u8StartKeyPressCunt = 0;
					return START_ID;
				}
			}
		}
		else
		{
			HAL_Timer_TIM4Stop();
			g_u8StartKeyPressCunt = 0;
			return ERROR_ID;
		}
		
	}
}

/**
 * @brief ��ʼ���������������
 * 
 * @param void 
 */
static void MID_KEYBOARD_Deal(void)		
{
	if(MID_KEYBOARD_KeyState() == START_ID)
	{
		g_u8Start = 1;
		g_u8StartFlag = 1;
	}
	else
	{
		HAL_Timer_TIM3Start(3);			//�������������ʱ��
	}
}



