#include "hal_timer.h"
#include "Global.h"
#include "mid_keyboard.h"

#define RCC_APBxPeriph_ClearTIM		RCC_APB1Periph_TIM3
#define ClearTIMx					TIM3
#define ClearIRQChannel				TIM3_IRQn

#define RCC_APBxPeriph_StartTIM		RCC_APB1Periph_TIM4
#define StartTIMx					TIM4
#define StartIRQChannel				TIM4_IRQn

uint8_t g_u8TIM3Flg = 0;
uint32_t g_u32TIM3Time = 0;
uint32_t g_u32TIM3Count = 0;
uint8_t g_u8TIM3RunState = 0;

uint8_t g_u8TIM4Flg = 0;
uint32_t g_u32TIM4Count = 0;
uint8_t g_u8TIM4RunState = 0;


/**
 * @brief 定时器3配置 清除按键
 * 
 * @param void 
 */
void HAL_Timer_TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_DeInit(ClearTIMx);

	TIM_TimeBaseStructure.TIM_Period = 35999;		//72MHz  ,1s , T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(35999+1)*(1999+1)/72MHz=1s
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ClearTIMx, &TIM_TimeBaseStructure);

	TIM_ClearFlag(ClearTIMx, TIM_FLAG_Update);
	TIM_ITConfig(ClearTIMx, TIM_IT_Update, ENABLE);
}

/**
 * @brief 定时器3NVIC配置
 * 
 * @param void 
 */
void HAL_Timer_TIM3_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = ClearIRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 定时器3初始化
 * 
 * @param void 
 */
void HAL_Timer_TIM3Init(void)
{
	HAL_Timer_TIM3_Config();
	HAL_Timer_TIM3_NVIC_Config();
}

/**
 * @brief 定时器3开启
 * 
 * @param u32Times 
 */
//min - max   0s -  86400s(24h)
void HAL_Timer_TIM3Start(uint32_t u32Times)
{
	if(g_u8TIM3RunState == FALSE)
	{
		g_u32TIM3Time = u32Times;
		g_u32TIM3Count = g_u32TIM3Time;
		g_u8TIM3Flg = FALSE;
		HAL_Timer_TIM3Init();
		TIM_Cmd(ClearTIMx, ENABLE);
		g_u8TIM3RunState = TRUE;
	}
}

void HAL_Timer_TIM3Stop(void)
{
	if(g_u8TIM3RunState == TRUE)
	{
		TIM_Cmd(ClearTIMx, DISABLE);
		g_u8TIM3RunState = FALSE;
		g_u32TIM3Count = 0;
		g_u32TIM3Time = 0;
	}
}

uint32_t HAL_Timer_TIM3_CountValue(void)
{
	return g_u32TIM3Count;
}


void TIM3_IRQHandler(void)
{

	if(TIM_GetFlagStatus(ClearTIMx, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearFlag(ClearTIMx, TIM_FLAG_Update);  

		if(g_u8ClearTimeFlag == 1)
		{
			if(CLEAR_KEY() == 0)
			{
				if(--g_u32TIM3Count == 0)
				{
					g_u8ClearFlag = 1;
					g_u32TIM3Count = g_u32TIM3Time;
					g_u8TIM3Flg = TRUE;
					g_u8TIM3RunState = FALSE;
					TIM_Cmd(ClearTIMx, DISABLE);
					g_u8ClearTimeFlag = 0;
				}
			}
			else
			{
				g_u8ClearTimeFlag = 0;
				g_u8ClearFlag = 0;
			}
		}
	}
}


/**
 * @brief 定时器4配置 开始按键
 * 
 * @param void 
 */
void HAL_Timer_TIM4_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APBxPeriph_StartTIM, ENABLE);

	TIM_DeInit(StartTIMx);

	TIM_TimeBaseStructure.TIM_Period = 35999;		//72MHz  ,1s , T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(35999+1)*(1999+1)/72MHz=1s
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(StartTIMx, &TIM_TimeBaseStructure);

	TIM_ClearFlag(StartTIMx, TIM_FLAG_Update);
	TIM_ITConfig(StartTIMx, TIM_IT_Update, ENABLE);
}

void HAL_Timer_TIM4_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = StartIRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void HAL_Timer_TIM4Init(void)
{
	HAL_Timer_TIM4_Config();
	HAL_Timer_TIM4_NVIC_Config();
}

void HAL_Timer_TIM4Start(void)
{
	if(g_u8TIM4RunState == FALSE)
	{
		HAL_Timer_TIM4Init();
		TIM_Cmd(StartTIMx, ENABLE);
		g_u8TIM4RunState = TRUE;
	}
}

void HAL_Timer_TIM4Stop(void)
{
	if(g_u8TIM4RunState == TRUE)
	{
		TIM_Cmd(StartTIMx, DISABLE);
		g_u8TIM4RunState = FALSE;
	}
}

void TIM4_IRQHandler(void)
{

	if(TIM_GetFlagStatus(StartTIMx, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearFlag(StartTIMx, TIM_FLAG_Update);  

		g_u8TIM4Flg = TRUE;
		if(g_u32TIM4Count++ >= 0xFFFFFFFF)
		{
			g_u32TIM4Count = 0;
		}
	}

}






