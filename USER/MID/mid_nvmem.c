#include "mid_nvmem.h"
#include "hal_uart.h"
#include "PUB_LIB.h"
#include <stdio.h>


/**
 * @brief flash��ȡ����
 * 
 * @param u32StartAddr 	Ҫ��ȡ����ʼ��ַ
 * @param u32Len 		Ҫ��ȡ�ĳ���
 * @param pu32DataBuff 	��ȡ��������
 * @param pu16NewData 	�ɼ������µ�ADֵ ���������ݲ��� �ò�������Ϊ��
 */
void MID_NVMEM_Read(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32DataBuff , uint16_t *pu16NewData)
{
    uint32_t i;
    uint32_t u32FlashStatus = 0;
	uint32_t NewCRCValue = 0;
	
    u32FlashStatus = HAL_NVMEM_Read(u32StartAddr, u32Len, pu32DataBuff);		// OK 0 NOK 1 ����1��ֵΪ��ʧ���ݵļ���ֵ

    if(u32FlashStatus == NOK)
    {
        if(HAL_NVMEM_Read(USER_BACKUP_MEMORY_START, u32Len, pu32DataBuff) == NOK)
        {
			printf("������������ȫ������������������ݣ�������");
			__set_FAULTMASK(1); 								//�ر������ж�
			NVIC_SystemReset();									//ϵͳ��λ
        }
    }

	if(pu16NewData != NULL)
	{
		if(u32FlashStatus > 1)
		{
			for (i = 0; i < 20;i++)
			{
				pu32DataBuff[i] = pu32DataBuff[i] + ((u32FlashStatus - 1) * (uint32_t)pu16NewData[i]);	//���������ʧ��������
			}

			NewCRCValue = PUB_LIB_CRC32((uint8_t *)pu32DataBuff, (u32Len - 1) * 4);	//����CRCУ��ֵ
			pu32DataBuff[20] = NewCRCValue;
		}
	}

}


/**
 * @brief flashд����
 * 
 * @param u32StartAddr Ҫд�����ʼ��ַ
 * @param u32Len 		д������ݳ���
 * @param pu32Databuff Ҫд�������
 */
void MID_NVMEM_Write(uint32_t u32StartAddr, uint32_t u32Len, uint32_t *pu32Databuff)
{
    HAL_NVMEM_Write(u32StartAddr, u32Len, pu32Databuff);
}
