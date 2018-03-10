#ifndef __MID_ELECTRIC_H
#define __MID_ELECTRIC_H

#include "hal_electric.h"
#include "hal_nvmem.h"

typedef struct ELECTRIC_MAH
{
	uint8_t *pu8Electric[20];
	uint8_t pu8ElecLen[20];
}ELECTRIC_mAh;


void MID_ELECTRIC_SaveVolt2Mem(void);
void MID_ELECTRIC_ClearVolt2Mem(void);
void MID_ELECTRIC_QuaryVolt2Mem(void);
void MID_ELECTRIC_Collect20Voltages(uint16_t *pu16VoltBuffer);
uint8_t MID_ELECTRIC_SelfInspection(void);
void MID_ELECTRIC_Collect12VAD(uint16_t *pu16VoltBuffer);
void MID_ELECTRIC_Collect3VAD(uint16_t *pu16VoltBuffer);
#endif

