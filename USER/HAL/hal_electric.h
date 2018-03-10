#ifndef __HAL_ELECTRIC_H
#define __HAL_ELECTRIC_H

#include "stm32f10x.h"

#define 	RESISTANCE 	 330


void HAL_ELECTRIC_Init(void);
uint32_t HAL_ELECTRIC_ConvertedValue(void);
uint8_t HAL_ELECTRIC_SelfInspection(void);
uint16_t HAL_ELECTRIC_ADAverageValue(void);
#endif

