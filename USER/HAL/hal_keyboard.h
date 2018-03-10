#ifndef __HAL_KEYBOARD_H
#define __HAL_KEYBOARD_H

#include "stm32f10x.h"



#define START_KEY() 		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define CLEAR_KEY() 		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
#define QUERY_KEY() 		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)



typedef enum {
	ERROR_ID = -1,
	START_ID = 0,
	CLEAR_ID = 1,
	QUERY_ID = 2
}KeyId;

void HAL_KEYBOARD_Init(void);
KeyId HAL_KEYBOARD_ISPRESSED(void);


#endif

