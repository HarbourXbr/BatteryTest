#ifndef __HAL_UART_H
#define __HAL_UART_H

#include "stm32f10x.h"

void HAL_UART_Init(void);
void HAL_SendStr(uint8_t *pu8Data,uint16_t u16Len);
void HAL_UART_Send(uint8_t *pu8Send, uint32_t u32Len);

#endif

