#ifndef __HAL_STMFLASH_H
#define __HAL_STMFLASH_H

#include "stm32f10x.h"

#define _1K_SIZE_ 					0x0400
/*FLASH每页大小*/
#define FLASH_PAGE_SIZE 			_1K_SIZE_	
/*FLASH总共的大小*/								
#define FLASH_SIZE 					128
/*FLASH扇区大小*/
#define FLASH_SECTOR_SIZE 			FLASH_PAGE_SIZE
/*FLASH起始地址*/
#define FLASH_START_ADDR 			0x08000000
/*FLASH结束地址*/
#define FLASH_END_ADDR 				(FLASH_START_ADDR + 1024 * FLASH_SIZE)

// #define FLASH_SECTOR_ADDR_LO 		(USER_BATTERY_MEMORY_SIZE - 1)
// #define FLASH_SECTOR_ADDR_HI 		(0xffff - FLASH_SECTOR_ADDR_LO)

/*用户区*/
#define USER_MEMORY_PAGE 			42
#define USER_DATA_MEMORY_START 		(FLASH_END_ADDR - USER_DATA_MEMORY_SIZE)
#define USER_DATA_MEMORY_SIZE 		(FLASH_PAGE_SIZE * USER_MEMORY_PAGE)
/*数据区*/
#define USER_BATTERY_MEMORY_PAGE	21
#define USER_BATTERY_MEMORY_START 	USER_DATA_MEMORY_START
#define USER_BATTERY_MEMORY_SIZE 	(FLASH_PAGE_SIZE * USER_BATTERY_MEMORY_PAGE)
#define USER_BATTERY_MEMORY_END 	(USER_DATA_MEMORY_START + USER_BATTERY_MEMORY_SIZE)
/*备份区*/
#define USER_BACKUP_PAGE			21
#define USER_BACKUP_MEMORY_START 	USER_BATTERY_MEMORY_END
#define USER_BACKUP_MEMORY_SIZE 	(FLASH_PAGE_SIZE * USER_BACKUP_PAGE)
#define USER_BACKUP_MEMORY_END 		(USER_BACKUP_MEMORY_START + USER_BACKUP_MEMORY_SIZE)
/*数据包大小*/
#define BATTERY_DATA_SIZE			USER_BATTERY_MEMORY_PAGE

typedef enum {
	OK = 0,
	NOK = 1,
} FLASH_STATUS;

uint32_t HAL_NVMEM_Read(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff);
FLASH_STATUS HAL_NVMEM_Write(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff);
void HAL_NVMEM_Erase(uint32_t u32StartAddr,uint32_t u32PageSize);
uint32_t HAL_NVMEM_GetCurrentWRAddr(uint32_t u32StartAddr);
uint32_t HAL_NVMEM_GetCurrentRDAddr(uint32_t u32StartAddr);
FLASH_STATUS HAL_NVMEM_VerifyData(uint32_t *pu32DataBuff, uint32_t u32Len);

void FlashRead(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff);
void FlashWrite(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff);
void FlashSectorErase(uint32_t u32StartAddr);
uint32_t HAL_NVMEM_GetRegionSize(uint32_t StartAddr);

#endif
