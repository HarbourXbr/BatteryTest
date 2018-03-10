#ifndef __MID_NVMEM_H
#define __MID_NVMEM_H

#include "hal_nvmem.h"

#define PNULL	((void*)0)

void MID_NVMEM_Read(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff , uint16_t *pu16NewData);
void MID_NVMEM_Write(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32Databuff);

#endif

