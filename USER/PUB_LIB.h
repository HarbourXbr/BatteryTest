#ifndef __PUB_LIB_H
#define __PUB_LIB_H

#include <stdint.h>
#include <string.h>


void PUB_LIB_MemSet(uint8_t *pu8Buf, uint8_t u8Value, uint32_t u32Len);
int8_t PUB_LIB_MemCmp(uint8_t *pu8Buf1, uint8_t *pu8Buf2, uint32_t u32Len);
void PUB_LIB_MemCpy(uint8_t *pu8Buf1, uint8_t *pu8Buf2, uint32_t u32Len);
int PUB_LIB_MemMem(uint8_t *pMem, int MemLen, uint8_t *pSubMem, int SubMemLen);
uint32_t PUB_LIB_Atoi(uint8_t *pu8ptr, uint8_t u8Len);
void PUB_LIB_itoa(uint32_t u32Val, uint8_t *pu8ptr, uint8_t *u8Len);
void PUB_LIB_Fatoia(uint8_t *pu8Buf1, uint8_t u8Len1, uint8_t *pu8Buf2, uint8_t *pu8Len2, uint8_t u8Dec);
void PUB_LIB_QuickSort(uint16_t *pu16Buffer,uint32_t u32Len);
void PUB_LIB_Reverse8(uint8_t *pu8In, uint8_t *pu8Out, uint32_t u32Len);
uint32_t PUB_LIB_CRC32(const uint8_t *pu8ptr, uint32_t u32Len);
#endif

