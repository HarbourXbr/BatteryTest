#include "hal_uart.h"
#include "stm32f10x.h"
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
#define RCC_APBxPeriph_UARTx		RCC_APB1Periph_USART2
#define RCC_APBxPeriph_GPIOx		RCC_APB2Periph_GPIOA
#define UARTx_GPIOx					GPIOA
#define UARTx						USART2
#define UARTx_Mode					USART_Mode_Tx
#define UARTx_Band					115200
  

void HAL_UART_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APBxPeriph_UARTx, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_GPIOx, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(UARTx_GPIOx, &GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate = UARTx_Band;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = UARTx_Mode;

//	USART_DeInit(UART4);
	
	USART_Init(UARTx, &USART_InitStruct);
	
	USART_Cmd(UARTx, ENABLE);
}

void HAL_UART_SendByte(uint8_t u8Byte)
{
	USART_SendData(UARTx, u8Byte);
	while (USART_GetFlagStatus(UARTx, USART_FLAG_TC) == RESET);
}

void HAL_UART_Send(uint8_t *pu8Send, uint32_t u32Len)
{
	uint16_t i = 0;

	while(u32Len-- != 0)
	{
		HAL_UART_SendByte(pu8Send[i++]);
	}
}


PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(UARTx, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UARTx, USART_FLAG_TC) == RESET)
  {}

  return ch;
}



