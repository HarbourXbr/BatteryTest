#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void Delay_Init(void);							    								   
void Delay_Us(u32 nus);
void Delay_Ms(u16 nms);


#endif 

