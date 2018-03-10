#include "Delay.h"
#include "misc.h"

static uint8_t  fac_us = 0;							//us延时倍乘数
static uint16_t fac_ms = 0;							//ms延时倍乘数


void Delay_Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000;				//是系统时钟的1/8

    fac_ms = (uint16_t)fac_us * 1000;
}

void Delay_Us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us; 					//时间加载
    SysTick->VAL = 0x00;        					//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒计时
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL = 0X00;      					 //清空计数器
}

void Delay_Ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms;				//时间加载(SysTick->LOAD是24bit)
    SysTick->VAL = 0x00;							//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒计时
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL = 0X00;       					//清空计数器
}








































