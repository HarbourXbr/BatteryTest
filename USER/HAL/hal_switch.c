#include "hal_switch.h"


#define RCC_APBxPeriph_MasterSW			RCC_APB2Periph_GPIOC
#define MasterSW_GPIOx					GPIOC
#define RCC_APBxPeriph_BatSW			(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD)

static void BatterySwitch_Config(void);
static void MasterSwitch_Config(void);

/**
 * @brief ADC采集开关初始化
 * 
 * @param void 
 */
void HAL_AllSwitch_Init(void)
{
	MasterSwitch_Config();
	BatterySwitch_Config();
}

/**
 * @brief ADC采集主开关IO配置
 * 
 * @param void 
 */
static void MasterSwitch_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
    RCC_APB2PeriphClockCmd(RCC_APBxPeriph_MasterSW, ENABLE); 
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(MasterSW_GPIOx, &GPIO_InitStruct);
	MasterSwitch_Off();
}

/**
 * @brief 电池开关IO配置
 * 
 * @param void 
 */
static void BatterySwitch_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_BatSW, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | 
								GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | 
								GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14|
								GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	BatteryAllSwitch_Off();
	
}


