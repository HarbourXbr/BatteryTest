#ifndef __HAL_SWITCH_H
#define __HAL_SWITCH_H

#include "MyConfig.h"

#define MasterSwitch_On()	GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define MasterSwitch_Off()	GPIO_SetBits(GPIOC, GPIO_Pin_3)

#define BatteryAllSwitch_Off()		do{	\
										GPIO_ResetBits(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | \
													GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | \
													GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| \
													GPIO_Pin_15);\
										GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);\
									}while(0)



#define BatterySwitchPinB(pinx)   	BIT_ADDR(GPIOB_ODR_Addr,pinx)   
#define BatterySwitchPinD(pinx)   	BIT_ADDR(GPIOD_ODR_Addr,pinx)


void HAL_AllSwitch_Init(void);

#endif

