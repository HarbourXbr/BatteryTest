#include "hal_nvmem.h"
#include "PUB_LIB.h"
#include <string.h>

/**
 * @brief flash读取函数
 * 
 * @param u32StartAddr 	读取数据的起始地址
 * @param u32Len 		读取数据的长度
 * @param pu32DataBuff 	读取出来的数据
 * @return FLASH_STATUS OK 成功
 * 						NOK 不成功
 */
uint32_t HAL_NVMEM_Read(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff)
{
	uint32_t CurrentReadAddr = 0;
	uint32_t i;
	uint32_t count = 0;

	if((u32StartAddr > FLASH_END_ADDR) && (u32StartAddr < USER_BATTERY_MEMORY_START))
	{
		return NOK;
	}

	CurrentReadAddr = HAL_NVMEM_GetCurrentRDAddr(u32StartAddr);							//获取当前读取地址的位置
	// u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);

	do
	{
		if(CurrentReadAddr == u32StartAddr)								//判断当前区域的地址是不是起始地址 如果是的话 直接从起始地址读取
		{
			for (i = 0; i < BATTERY_DATA_SIZE; i++)
			{
				if(*(__IO uint32_t *)CurrentReadAddr == 0xffffffff)
				{
					CurrentReadAddr += 4;
				}
				else
				{
					break;
				}
			}
			if(i == BATTERY_DATA_SIZE)
			{
				return OK;
			}
			else
			{
				FlashRead(u32StartAddr, u32Len, pu32DataBuff); //返回是否检验成功
				return HAL_NVMEM_VerifyData(pu32DataBuff, u32Len);
			}
				
		}
		FlashRead(CurrentReadAddr, u32Len, pu32DataBuff);
		CurrentReadAddr -= BATTERY_DATA_SIZE * 4;
		count++;
	} while (HAL_NVMEM_VerifyData(pu32DataBuff, u32Len));				//确认读取的数据是否正确

	if(count > 1)
	{
		return count;						//返回丢失的数据或者是错误的数据的次数
	}
	
	return OK;
}

/**
 * @brief flash写数据
 * 
 * @param u32StartAddr 				写的起始地址
 * @param u32Len 					要写入数据的长度
 * @param pu32DataBuff 				要写入的数据
 * @return FLASH_STATUS 					写入是否成功
 */
FLASH_STATUS HAL_NVMEM_Write(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff)
{
	uint32_t u32CurrentWRAddr = 0;
	uint32_t pu32ReadBuffer[BATTERY_DATA_SIZE];
	uint32_t u32RegionSize = 0;

	if(u32StartAddr > FLASH_END_ADDR)
	{
		return NOK;
	}

	if(u32Len == 0)
	{
		return OK;
	}

	u32CurrentWRAddr = HAL_NVMEM_GetCurrentWRAddr(u32StartAddr);						//当前写入位置
	u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);								//获取数据区的大小

	if((u32CurrentWRAddr >= USER_DATA_MEMORY_START) && (u32CurrentWRAddr <= FLASH_END_ADDR))
	{
		do
		{
			if(u32CurrentWRAddr >= (u32StartAddr + u32RegionSize))						//判断当前区域是否写满 写满擦除整个区域
			{
				u32CurrentWRAddr = u32StartAddr;
				HAL_NVMEM_Erase(u32StartAddr, 21);
			}
			FlashWrite(u32CurrentWRAddr, u32Len, pu32DataBuff);
			HAL_NVMEM_Read(u32CurrentWRAddr, BATTERY_DATA_SIZE, pu32ReadBuffer);
			u32CurrentWRAddr += BATTERY_DATA_SIZE * 4;
		}
		while (HAL_NVMEM_VerifyData(pu32ReadBuffer, BATTERY_DATA_SIZE));	//没写满，从当前地址继续写

		return OK;
	}
	else
	{
		return NOK;
	}	
}

/**
 * @brief 确认数据是否正确 （比较数据包的CRC校验值和数据的CRC的校验值）
 * 
 * @param pu32DataBuff 
 * @param u32Len 
 * @return uint32_t 0 正确 1 失败
 */
FLASH_STATUS HAL_NVMEM_VerifyData(uint32_t *pu32DataBuff, uint32_t u32Len)
{
	uint32_t i;
	uint32_t TmpBuff[BATTERY_DATA_SIZE] = {0};
	uint32_t u32PreCrc32 = 0;
	uint32_t u32CurrentCrc32 = 0;

	u32PreCrc32 = pu32DataBuff[BATTERY_DATA_SIZE -1];

	for (i = 0; i < (u32Len -1);i++)
	{
		TmpBuff[i] = pu32DataBuff[i];
	}
	
	u32CurrentCrc32 = PUB_LIB_CRC32((uint8_t *)TmpBuff, (u32Len -1) * 4);

	if(u32PreCrc32 == u32CurrentCrc32)
	{
		return OK;
	}
	else
	{
		return NOK;
	}
}

/**
 * @brief 擦除整个数据区的数据
 * 
 * @param u32StartAddr 擦除数据的起始地址
 */
void HAL_NVMEM_Erase(uint32_t u32StartAddr,uint32_t u32PageSize)
{
	uint32_t i ;
	for(i = 0; i < u32PageSize; i++)
	{
		FLASH_ErasePage(u32StartAddr + (i * FLASH_SECTOR_SIZE));				
	}
}

void FlashRead(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff)
{
	int i;
	for(i = 0; i <  u32Len; i++, u32StartAddr += 4)
	{
		pu32DataBuff[i] = *(__IO uint32_t *) u32StartAddr;
	}
}

void FlashWrite(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff)
{
	uint32_t i;

	for(i = 0; i <  u32Len; i++, u32StartAddr += 4)
	{
		FLASH_ProgramWord(u32StartAddr, pu32DataBuff[i]);
	}
}

void FlashSectorErase(uint32_t u32StartAddr)
{
	FLASH_ErasePage(u32StartAddr);
}

/**
 * @brief 获取当前写的起始地址
 * 
 * @param u32StartAddr 
 * @return uint32_t 返回当前写入的地址
 */
uint32_t HAL_NVMEM_GetCurrentWRAddr(uint32_t u32StartAddr)
{
    uint32_t i, j;
    uint32_t count = 0;
    uint32_t CurrentWriteAddr = 0;
	uint32_t u32RegionSize = 0;

	u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);
	
	if(u32RegionSize == 0)
	{
		return NULL;
	}

    for(i = 0; i < u32RegionSize; i += BATTERY_DATA_SIZE * 4)
	{
        for (j = 0; j < BATTERY_DATA_SIZE; j++)
        {
            if(*(__IO uint32_t *)u32StartAddr == 0xffffffff)
            {
                count++;
            }
            else
            {
                count = 0;
            }
			u32StartAddr += 4;
		}
        if(count == BATTERY_DATA_SIZE)
        {
			CurrentWriteAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4);	//返回当前可写入数据的首地址 地址 - 数据包大小 * 每个数据字节大小
			return CurrentWriteAddr;
        }
    }
    return u32StartAddr;	//没有空闲区，则返回该区域的累加地址
}

/**
 * @brief 获取当前读取的地址
 * 
 * @param u32StartAddr 
 * @return uint32_t 返回读取地址
 */
uint32_t HAL_NVMEM_GetCurrentRDAddr(uint32_t u32StartAddr)
{
    uint32_t i, j;
    uint32_t count = 0;
    uint32_t CurrentReadAddr = 0;
	uint32_t u32RegionSize = 0;

	u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);		//获取地址所在区域大小

	if(u32RegionSize == 0)
	{
		return NULL;
	}

	for(i = 0; i < u32RegionSize; i += BATTERY_DATA_SIZE * 4)
	{
		if(u32StartAddr >= FLASH_END_ADDR)
		{
			CurrentReadAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4);		//如果该区域都为满数据，则返回该区域最后后一组数据的首地址
			return CurrentReadAddr;
		}
        for (j = 0; j < BATTERY_DATA_SIZE; j++)					//判断该数据块是否都是空
        {
            if(*(__IO uint32_t *)u32StartAddr == 0xffffffff)
            {
                count++;
            }
            else
            {
                count = 0;
            }
			u32StartAddr += 4;
        }
        if(count == BATTERY_DATA_SIZE)
        {
			if(i == 0)
			{
				return u32StartAddr - (BATTERY_DATA_SIZE * 4);		//如果该区域第一块区域就为空 则返回这块区域的首地址
			}
			CurrentReadAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4 * 2);	//返回空区域的前面数据的首地址
			return CurrentReadAddr;
        }
    }
	CurrentReadAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4);		//如果该区域都为满数据，则返回该区域最后后一组数据的首地址
    return CurrentReadAddr;
}


/**
 * @brief 获取存储区对应的存储区大小
 * 
 * @param StartAddr 
 * @return uint32_t 
 */
uint32_t HAL_NVMEM_GetRegionSize(uint32_t StartAddr)
{
	if((StartAddr >= USER_BATTERY_MEMORY_START) && (StartAddr < USER_BATTERY_MEMORY_END))
	{
		return USER_BATTERY_MEMORY_SIZE;
	}
	else if((StartAddr >= USER_BACKUP_MEMORY_START) && (StartAddr < USER_BACKUP_MEMORY_END))
	{
		return USER_BACKUP_MEMORY_SIZE;
	}
	else
	{
		return 0;
	}
}
