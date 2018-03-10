#ifndef __HAL_LED_H
#define __HAL_LED_H

#include"MyConfig.h"

#define LED_AllOff()		do{ \
								GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | \
													GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | \
													GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| \
													GPIO_Pin_15); \
								GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); \
							}while(0)

#define LED_AllOn()			do{ \
								GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | \
													GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | \
													GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| \
													GPIO_Pin_15); \
								GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); \
							}while(0)



#define BatteryThresholdLedA(bitx)	BIT_ADDR(GPIOA_ODR_Addr, bitx)
#define BatteryThresholdLedE(bitx)	BIT_ADDR(GPIOE_ODR_Adrr, bitx)

void HAL_LED_Init(void);
void LED_TWINKLE(void);

#endif






