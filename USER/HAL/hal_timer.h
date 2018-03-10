#ifndef __HAL_TIMER_H
#define __HAL_TIMER_H

#include "stm32f10x.h"

typedef enum{
	FALSE = 0,
	TRUE = 1,
}STATUS;

extern uint8_t g_u8TIM3Flg;
extern uint8_t g_u8TIM4Flg;



void HAL_Timer_TIM3Init(void);
void HAL_Timer_TIM3Start(uint32_t u32Times);
void HAL_Timer_TIM3Stop(void);
uint32_t HAL_Timer_TIM3_CountValue(void);

void HAL_Timer_TIM4Init(void);
void HAL_Timer_TIM4Start(void);
void HAL_Timer_TIM4Stop(void);

void HAL_Timer_TIM7Init(void);
void HAL_Timer_TIM7Start(uint32_t u32Times);
void HAL_Timer_TIM7Stop(void);

void HAL_Timer_TIM6DeayMS(uint16_t u16DelayTime);

#endif

