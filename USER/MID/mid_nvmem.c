#include "mid_nvmem.h"
#include "hal_uart.h"
#include "PUB_LIB.h"
#include <stdio.h>


/**
 * @brief flash读取函数
 * 
 * @param u32StartAddr 	要读取的起始地址
 * @param u32Len 		要读取的长度
 * @param pu32DataBuff 	读取到的数据
 * @param pu16NewData 	采集到的新的AD值 用来做数据补偿 该参数可以为空
 */
void MID_NVMEM_Read(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff , uint16_t *pu16NewData)
{
    uint32_t i;
    uint32_t u32FlashStatus = 0;
	uint32_t NewCRCValue = 0;
	
    u32FlashStatus = HAL_NVMEM_Read(u32StartAddr, u32Len, pu32DataBuff);		// OK 0 NOK 1 大于1的值为丢失数据的计数值

    if(u32FlashStatus == NOK)
    {
        if(HAL_NVMEM_Read(USER_BACKUP_MEMORY_START, u32Len, pu32DataBuff) == NOK)
        {
			printf("！！！！数据全部出错，请清除所有数据！！！！");
			__set_FAULTMASK(1); 								//关闭所有中断
			NVIC_SystemReset();									//系统复位
        }
    }

	if(pu16NewData != NULL)
	{
		if(u32FlashStatus > 1)
		{
			for (i = 0; i < 20;i++)
			{
				pu32DataBuff[i] = pu32DataBuff[i] + ((u32FlashStatus - 1) * (uint32_t)pu16NewData[i]);	//补偿错误或丢失数据数据
			}

			NewCRCValue = PUB_LIB_CRC32((uint8_t *)pu32DataBuff, (u32Len - 1) * 4);	//更新CRC校验值
			pu32DataBuff[20] = NewCRCValue;
		}
	}

}


/**
 * @brief flash写函数
 * 
 * @param u32StartAddr 要写入的起始地址
 * @param u32Len 		写入的数据长度
 * @param pu32Databuff 要写入的数据
 */
void MID_NVMEM_Write(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32Databuff)
{
    HAL_NVMEM_Write(u32StartAddr, u32Len, pu32Databuff);
}
