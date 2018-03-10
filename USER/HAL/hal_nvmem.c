#include "hal_nvmem.h"
#include "PUB_LIB.h"
#include <string.h>

/**
 * @brief flash��ȡ����
 * 
 * @param u32StartAddr 	��ȡ���ݵ���ʼ��ַ
 * @param u32Len 		��ȡ���ݵĳ���
 * @param pu32DataBuff 	��ȡ����������
 * @return FLASH_STATUS OK �ɹ�
 * 						NOK ���ɹ�
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

	CurrentReadAddr = HAL_NVMEM_GetCurrentRDAddr(u32StartAddr);							//��ȡ��ǰ��ȡ��ַ��λ��
	// u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);

	do
	{
		if(CurrentReadAddr == u32StartAddr)								//�жϵ�ǰ����ĵ�ַ�ǲ�����ʼ��ַ ����ǵĻ� ֱ�Ӵ���ʼ��ַ��ȡ
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
				FlashRead(u32StartAddr, u32Len, pu32DataBuff); //�����Ƿ����ɹ�
				return HAL_NVMEM_VerifyData(pu32DataBuff, u32Len);
			}
				
		}
		FlashRead(CurrentReadAddr, u32Len, pu32DataBuff);
		CurrentReadAddr -= BATTERY_DATA_SIZE * 4;
		count++;
	} while (HAL_NVMEM_VerifyData(pu32DataBuff, u32Len));				//ȷ�϶�ȡ�������Ƿ���ȷ

	if(count > 1)
	{
		return count;						//���ض�ʧ�����ݻ����Ǵ�������ݵĴ���
	}
	
	return OK;
}

/**
 * @brief flashд����
 * 
 * @param u32StartAddr 				д����ʼ��ַ
 * @param u32Len 					Ҫд�����ݵĳ���
 * @param pu32DataBuff 				Ҫд�������
 * @return FLASH_STATUS 					д���Ƿ�ɹ�
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

	u32CurrentWRAddr = HAL_NVMEM_GetCurrentWRAddr(u32StartAddr);						//��ǰд��λ��
	u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);								//��ȡ�������Ĵ�С

	if((u32CurrentWRAddr >= USER_DATA_MEMORY_START) && (u32CurrentWRAddr <= FLASH_END_ADDR))
	{
		do
		{
			if(u32CurrentWRAddr >= (u32StartAddr + u32RegionSize))						//�жϵ�ǰ�����Ƿ�д�� д��������������
			{
				u32CurrentWRAddr = u32StartAddr;
				HAL_NVMEM_Erase(u32StartAddr, 21);
			}
			FlashWrite(u32CurrentWRAddr, u32Len, pu32DataBuff);
			HAL_NVMEM_Read(u32CurrentWRAddr, BATTERY_DATA_SIZE, pu32ReadBuffer);
			u32CurrentWRAddr += BATTERY_DATA_SIZE * 4;
		}
		while (HAL_NVMEM_VerifyData(pu32ReadBuffer, BATTERY_DATA_SIZE));	//ûд�����ӵ�ǰ��ַ����д

		return OK;
	}
	else
	{
		return NOK;
	}	
}

/**
 * @brief ȷ�������Ƿ���ȷ ���Ƚ����ݰ���CRCУ��ֵ�����ݵ�CRC��У��ֵ��
 * 
 * @param pu32DataBuff 
 * @param u32Len 
 * @return uint32_t 0 ��ȷ 1 ʧ��
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
 * @brief ��������������������
 * 
 * @param u32StartAddr �������ݵ���ʼ��ַ
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
 * @brief ��ȡ��ǰд����ʼ��ַ
 * 
 * @param u32StartAddr 
 * @return uint32_t ���ص�ǰд��ĵ�ַ
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
			CurrentWriteAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4);	//���ص�ǰ��д�����ݵ��׵�ַ ��ַ - ���ݰ���С * ÿ�������ֽڴ�С
			return CurrentWriteAddr;
        }
    }
    return u32StartAddr;	//û�п��������򷵻ظ�������ۼӵ�ַ
}

/**
 * @brief ��ȡ��ǰ��ȡ�ĵ�ַ
 * 
 * @param u32StartAddr 
 * @return uint32_t ���ض�ȡ��ַ
 */
uint32_t HAL_NVMEM_GetCurrentRDAddr(uint32_t u32StartAddr)
{
    uint32_t i, j;
    uint32_t count = 0;
    uint32_t CurrentReadAddr = 0;
	uint32_t u32RegionSize = 0;

	u32RegionSize = HAL_NVMEM_GetRegionSize(u32StartAddr);		//��ȡ��ַ���������С

	if(u32RegionSize == 0)
	{
		return NULL;
	}

	for(i = 0; i < u32RegionSize; i += BATTERY_DATA_SIZE * 4)
	{
		if(u32StartAddr >= FLASH_END_ADDR)
		{
			CurrentReadAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4);		//���������Ϊ�����ݣ��򷵻ظ���������һ�����ݵ��׵�ַ
			return CurrentReadAddr;
		}
        for (j = 0; j < BATTERY_DATA_SIZE; j++)					//�жϸ����ݿ��Ƿ��ǿ�
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
				return u32StartAddr - (BATTERY_DATA_SIZE * 4);		//����������һ�������Ϊ�� �򷵻����������׵�ַ
			}
			CurrentReadAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4 * 2);	//���ؿ������ǰ�����ݵ��׵�ַ
			return CurrentReadAddr;
        }
    }
	CurrentReadAddr = u32StartAddr - (BATTERY_DATA_SIZE * 4);		//���������Ϊ�����ݣ��򷵻ظ���������һ�����ݵ��׵�ַ
    return CurrentReadAddr;
}


/**
 * @brief ��ȡ�洢����Ӧ�Ĵ洢����С
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
