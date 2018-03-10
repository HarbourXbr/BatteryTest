#include "mid_electric.h"
#include "mid_keyboard.h"
#include "mid_nvmem.h"
#include "Delay.h"
#include "hal_switch.h"
#include "hal_uart.h"
#include "hal_led.h"
#include <string.h>
#include "Global.h"
#include "PUB_LIB.h"
#include <stdio.h>

#define THRESHOLD12V_VALUE 		2867
#define THRESHOLD3V_VALUE 		717
#define SI_THRESHOLD12V_VALUE 	3315
#define SI_THRESHOLD3V_VALUE	828
#define FULLYCHARGED_12V_VALUE	4095
#define FULLYCHARGED_3V_VALUE	1100

uint8_t g_u32Bat12VFlag = 0;
uint8_t g_u32Bat3VFlag = 0;


uint32_t count = 0;
/**
 * @brief	保存AD值到flash区中 
 * 
 * @param void 
 */

void MID_ELECTRIC_SaveVolt2Mem(void) 
{
	uint32_t i;
	uint32_t crc = 0;
	uint32_t u32TmpVoltBuf[BATTERY_DATA_SIZE] = {0};
	uint32_t u32SumVoltBuf[BATTERY_DATA_SIZE] = {0};
	uint16_t u16TmpVoltbuf[BATTERY_DATA_SIZE] = {0};


	
	MID_ELECTRIC_Collect20Voltages(u16TmpVoltbuf);				//得到当前AD值
	MID_NVMEM_Read(USER_BATTERY_MEMORY_START, BATTERY_DATA_SIZE, u32TmpVoltBuf, u16TmpVoltbuf); //读取上一组ad数据
	for (i = 0; i < (BATTERY_DATA_SIZE -1) ; i++)
	{
		if (u32TmpVoltBuf[i] == 0xffffffff)
		{
			u32TmpVoltBuf[i] = 0;
		}
		u32SumVoltBuf[i] = u16TmpVoltbuf[i] + u32TmpVoltBuf[i]; //累加ad值
		//		printf("\r\n B%02d  %u AD\r\n", i + 1, u32TmpVoltBuf[i]);
	}

	crc = PUB_LIB_CRC32((uint8_t *)u32SumVoltBuf, (BATTERY_DATA_SIZE - 1) * 4);
	u32SumVoltBuf[20] = crc;
	
	MID_NVMEM_Write(USER_BATTERY_MEMORY_START, BATTERY_DATA_SIZE, u32SumVoltBuf); //将ad值写入数据区中
	if(count++ > 2)
	{
		MID_NVMEM_Write(USER_BACKUP_MEMORY_START, BATTERY_DATA_SIZE, u32SumVoltBuf);	//将ad值写入备份区
	}
}

/**
 * @brief 清除数据区和备份区函数
 * 
 * @param void 
 */
void MID_ELECTRIC_ClearVolt2Mem(void) 
{
	HAL_NVMEM_Erase(USER_BATTERY_MEMORY_START, USER_BATTERY_MEMORY_PAGE);
	HAL_NVMEM_Erase(USER_BACKUP_MEMORY_START, USER_BACKUP_PAGE);
	count = 0;
}


/**
 * @brief 查询mah打印到串口
 * 
 * @param void 
 */
void MID_ELECTRIC_QuaryVolt2Mem(void)
{

	uint16_t i;
	uint32_t u32VoltBuffer[BATTERY_DATA_SIZE] = {0};
	float fmahBuffer = 0;
	MID_NVMEM_Read(USER_BATTERY_MEMORY_START, BATTERY_DATA_SIZE, u32VoltBuffer, NULL);

	for (i = 0; i < 20; i++)
	{
		if (u32VoltBuffer[i] == 0xffffffff)
		{
			u32VoltBuffer[i] = 0;
		}
		//		u32mAhBuffer[i] = u32VoltBuffer[i] * 3300 / 4095 * 4 /	RESISTANCE	/ 360;
		fmahBuffer = ((float)u32VoltBuffer[i] / 4095 * 3300 + 120) * 3.45 / RESISTANCE / 360; //毫安时计算公式
		printf("\r\n  B%02d  %.3f  mah\r\n", i + 1, fmahBuffer);
	}
}

/**
 * @brief 收集20组电池AD值
 * 
 * @param pu16VoltBuffer 
 */
void MID_ELECTRIC_Collect20Voltages(uint16_t *pu16VoltBuffer) 
{
	MasterSwitch_On();
	if (g_u32Battery12VFlag == 1)
	{
		MID_ELECTRIC_Collect12VAD(pu16VoltBuffer);
	}
	if (g_u32Battery3VFlag == 1)
	{
		MID_ELECTRIC_Collect3VAD(pu16VoltBuffer);
	}
}


/**
 * @brief 收集12V电池AD值
 * 
 * @param pu16VoltBuffer 
 */
void MID_ELECTRIC_Collect12VAD(uint16_t *pu16VoltBuffer)
{
	uint8_t i;
	uint8_t u8NoPwrCnt = 0;
	for (i = 0; i < 20; i++)
	{
		if (i < 16)
		{
			BatterySwitchPinD(i) = 1; //PD0 ~ 15
			Delay_Ms(500);
			pu16VoltBuffer[i] = HAL_ELECTRIC_ADAverageValue();
			if (pu16VoltBuffer[i] <= THRESHOLD12V_VALUE)
			{
				pu16VoltBuffer[i] = 0;
				BatteryThresholdLedE(i) = 0;
			}

			BatterySwitchPinD(i) = 0;
		}
		else
		{
			Delay_Ms(500);
			BatterySwitchPinB(i - 8) = 1; //PB2 ~ 5
			pu16VoltBuffer[i] = HAL_ELECTRIC_ADAverageValue();
			if (pu16VoltBuffer[i] <= THRESHOLD12V_VALUE)
			{
				pu16VoltBuffer[i] = 0;
				BatteryThresholdLedA(i - 12) = 0;
			}

			BatterySwitchPinB(i - 8) = 0;
		}
	}
	if(u8NoPwrCnt == 20)
	{
		MasterSwitch_Off();
		g_u8StartFlag = 0;
		printf("\r\n！！电池采集完毕！！\r\n");
	}
}

/**
 * @brief 收集3V电池AD值
 * 
 * @param pu16VoltBuffer 
 */
void MID_ELECTRIC_Collect3VAD(uint16_t *pu16VoltBuffer)
{
	uint8_t i;
	uint8_t u8NoPwrCnt = 0;
	for (i = 0; i < 20; i++)
	{
		if (i < 16)
		{
			BatterySwitchPinD(i) = 1; //PB0 ~ 15
			Delay_Ms(500);
			pu16VoltBuffer[i] = HAL_ELECTRIC_ADAverageValue();
			if (pu16VoltBuffer[i] <= THRESHOLD3V_VALUE)
			{
				pu16VoltBuffer[i] = 0;
				BatteryThresholdLedE(i) = 0;
				u8NoPwrCnt++;
			}

			BatterySwitchPinD(i) = 0;
		}
		else
		{
			BatterySwitchPinB(i - 8) = 1; //PC5 ~ 8
			Delay_Ms(500);
			pu16VoltBuffer[i] = HAL_ELECTRIC_ADAverageValue();
			if (pu16VoltBuffer[i] <= THRESHOLD3V_VALUE)
			{
				pu16VoltBuffer[i] = 0;
				BatteryThresholdLedA(i - 12) = 0;
				u8NoPwrCnt++;
			}

			BatterySwitchPinB(i - 8) = 0;
		}
	}
	if(u8NoPwrCnt == 20)
	{
		MasterSwitch_Off();
		g_u8StartFlag = 0;
		printf("\r\n！！电池采集完毕！！\r\n");
	}
}

/**
 * @brief AD首次上电自检测
 * 
 * @param void 
 * @return uint8_t 0 成功 1 失败
 */
uint8_t MID_ELECTRIC_SelfInspection(void)
{
	uint8_t i = 0;
	uint32_t ADCValue = 0;
	if((*(__IO uint32_t *)USER_BATTERY_MEMORY_START == 0xffffffff) && (g_u32Battery3VFlag == 0) && (g_u32Battery12VFlag == 0) && (*(__IO uint32_t *)USER_BACKUP_MEMORY_START == 0xffffffff))
	{
		MasterSwitch_On();
		for (i = 0; i < 20; ADCValue = 0, i++)
		{
			if(i < 16)
			{
				BatterySwitchPinD(i) = 1;
				Delay_Ms(200);
				ADCValue = HAL_ELECTRIC_ADAverageValue();
				if((ADCValue >= SI_THRESHOLD12V_VALUE) && (ADCValue <= FULLYCHARGED_12V_VALUE))
				{
					g_u32Bat12VFlag = 1;
					BatteryThresholdLedE(i) = 0;
				}
				else if((ADCValue >= SI_THRESHOLD3V_VALUE) && (ADCValue <= FULLYCHARGED_3V_VALUE))
				{
					g_u32Bat3VFlag = 1;
					BatteryThresholdLedE(i) = 0;
				}
				else
				{
					BatteryThresholdLedE(i) = 1;
					return 1;
				}
				BatterySwitchPinD(i) = 0;
			}
			else
			{
				BatterySwitchPinB(i - 8) = 1;
				Delay_Ms(200);
				ADCValue = HAL_ELECTRIC_ADAverageValue();
				if((ADCValue >= SI_THRESHOLD12V_VALUE) && (ADCValue <= FULLYCHARGED_12V_VALUE))
				{
					g_u32Bat12VFlag = 1;
					BatteryThresholdLedA(i - 12) = 0;
				}
				else if((ADCValue >= SI_THRESHOLD3V_VALUE) && (ADCValue <= FULLYCHARGED_3V_VALUE))
				{
					g_u32Bat3VFlag = 1;
					BatteryThresholdLedA(i - 12) = 0;
				}
				else
				{
					BatteryThresholdLedA(i - 12) = 1;
					return 1;
				}
				BatterySwitchPinB(i - 8) = 0;
			}
			
		}
		if(g_u32Bat12VFlag == 1)
		{
			g_u32Battery12VFlag = 1;
		}
		if(g_u32Bat3VFlag == 1)
		{
			g_u32Battery3VFlag = 1;
		}
			
		MasterSwitch_Off();
		printf("\r\n！！首次上电检测成功 ！！\r\n");
		Delay_Ms(500);
		LED_AllOff();
		printf("\r\n！！请按下开始按键 ！！\n");
		return 0;
	}
	else
	{
		MasterSwitch_On();
		BatterySwitchPinD(0) = 1;
		Delay_Ms(200);
		ADCValue = HAL_ELECTRIC_ADAverageValue();
		MasterSwitch_Off();
		BatterySwitchPinD(0) = 0;
		if(ADCValue <= FULLYCHARGED_3V_VALUE)
		{
			g_u32Battery3VFlag = 1;
		}
		else
		{
			g_u32Battery12VFlag = 1;
		}
		printf("\r\n！！请按下开始按键 ！！\n");
		return 0;
	}
}

//void MID_ELECTRIC_CheckBat(void)
//{
//	uint32_t ADCValue = 0;
//	
//	MasterSwitch_On();
//	BatterySwitchPinD(0) = 1;
//	Delay_Ms(200);
//	ADCValue = HAL_ELECTRIC_ADAverageValue();
//	MasterSwitch_Off();
//	BatterySwitchPinD(0) = 0;
//	if(ADCValue <= FULLYCHARGED_3V_VALUE)
//	{
//		g_u32Battery3VFlag = 1;
//	}
//	else
//	{
//		g_u32Battery12VFlag = 1;
//	}
//}

