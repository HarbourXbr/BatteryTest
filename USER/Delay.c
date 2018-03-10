#include "Delay.h"
#include "misc.h"

static uint8_t  fac_us = 0;							//us��ʱ������
static uint16_t fac_ms = 0;							//ms��ʱ������


void Delay_Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
    fac_us = SystemCoreClock / 8000000;				//��ϵͳʱ�ӵ�1/8

    fac_ms = (uint16_t)fac_us * 1000;
}

void Delay_Us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us; 					//ʱ�����
    SysTick->VAL = 0x00;        					//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����ʱ
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL = 0X00;      					 //��ռ�����
}

void Delay_Ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms;				//ʱ�����(SysTick->LOAD��24bit)
    SysTick->VAL = 0x00;							//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����ʱ
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL = 0X00;       					//��ռ�����
}








































