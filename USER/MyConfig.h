#ifndef __MYCONFIG_H
#define __MYCONFIG_H

#include "stm32f10x.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr    			(GPIOA_BASE + 12)
#define GPIOB_ODR_Addr    			(GPIOB_BASE + 12) //0x40010C0C 
#define GPIOC_ODR_Addr   			(GPIOC_BASE + 12) //0x4001100C 
#define GPIOD_ODR_Addr				(GPIOD_BASE + 12)
#define GPIOE_ODR_Adrr				(GPIOE_BASE + 12)

#endif
