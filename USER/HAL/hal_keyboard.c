#include "hal_keyboard.h"
#include "Global.h"
#include "Delay.h"

#define KEY_GPIOx					GPIOC
#define RCC_APBxPeriph_KEY			(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO)

static void KEYBOARD_NVICCfg(void);
static void KEYBOARD_GPIOCfg(void);
static void KEYBOARD_EXTICfg(void);



/**
 * @brief 按键初始化
 * 
 * @param void 
 */
void HAL_KEYBOARD_Init(void)
{
	KEYBOARD_GPIOCfg();
	KEYBOARD_EXTICfg();
	KEYBOARD_NVICCfg();
}


/**
 * @brief 按键IO初始化
 * 
 * @param void 
 */
static void KEYBOARD_GPIOCfg(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_KEY, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(KEY_GPIOx, &GPIO_InitStruct);
}


/**
 * @brief 查询按键和清除按键外部中断配置
 * 
 * @param void 
 */
static void KEYBOARD_EXTICfg(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearITPendingBit(EXTI_Line0);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);		//查询按键外部中断配置

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);			//擦除按键外部中断配置

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
 * @brief 按键NVIC配置 查询中断比清除中断优先级高
 * 
 * @param void 
 */
static void KEYBOARD_NVICCfg(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 开始按键确认按下
 * 
 * @param void 
 * @return KeyId 
 */
KeyId HAL_KEYBOARD_ISPRESSED(void)
{
	if(START_KEY() == 0)
	{
		return START_ID;
	}
	else
	{
		return ERROR_ID;
	}
}

/**
 * @brief 查询按键中断服务函数
 * 
 * @param void 
 */
void EXTI2_IRQHandler(void)
{
	if(QUERY_KEY() == 0)
	{
		g_u8QueryUartFlag = 1;				//查询按键是否按下并将标志置1
		
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/**
 * @brief 清除按键中断服务函数
 * 
 * @param void 
 */
void EXTI0_IRQHandler(void)
{
	if(CLEAR_KEY() == 0)
	{
		Delay_Ms(100);
		if(CLEAR_KEY() == 0)
		{
			g_u8ClearTimeFlag = 1;				//擦除按键是否按下并将对应定时器标志置1
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

