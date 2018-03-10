#include "hal_led.h"
#include "Delay.h"

#define RCC_APBxPeriph_LED		(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA)

/**
 * @brief LED初始化函数
 * 
 * @param void 
 */
void HAL_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_LED, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | 
								GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | 
								GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14|
								GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	LED_AllOff();
	
}


/**
 * @brief LED闪烁函数
 * 
 * @param void 
 */
void LED_TWINKLE(void)
{
	int i;
	for(i = 0; i < 2; i++)
	{
		LED_AllOn();
		Delay_Ms(300);
		LED_AllOff();
		Delay_Ms(300);
	}			
}

