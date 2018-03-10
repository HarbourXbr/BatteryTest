#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "stm32f10x.h"

extern uint32_t g_u32Battery12VFlag;
extern uint32_t g_u32Battery3VFlag;
extern uint8_t g_u8QueryUartFlag;
extern uint8_t g_u8ClearTimeFlag;
extern uint8_t g_u8ClearFlag;
extern __IO uint8_t g_u8StartFlag;
extern uint8_t g_u8Start;
extern uint32_t g_pu32BrownoutDetectADCVal[20];
extern uint8_t g_u8ADCSelfInspectionFlg;
#endif
