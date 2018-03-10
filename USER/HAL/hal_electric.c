#include "hal_electric.h"
#include "PUB_LIB.h"
#include "Global.h"


#define ADCx							ADC1
#define DMAx							DMA1
#define DMAx_Channelx 					DMA1_Channel1	
#define ADC_Channelx					ADC_Channel_0
#define RCC_AHBPeriph_DMAx				RCC_AHBPeriph_DMA1
#define RCC_APBxPeriph_ADCx				(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA)
#define ADC_PCLKx_Divx					RCC_PCLK2_Div6
#define ADC_GPIOx						GPIOA

#define ADC1_DR_Address ((uint32_t)0x40012400 + 0x4c)

#define CONVERTED_VALUE_LEN 10
uint16_t g_pu16ADC_ConverValue[CONVERTED_VALUE_LEN];

static void HAL_ELECTRIC_ADC1_Mode_Config(void);
static void HAL_ELECTRIC_ADC1_GPIO_Config(void);

/**
 * @brief ADC��ʼ��
 * 
 * @param void 
 */
void HAL_ELECTRIC_Init(void)
{
	HAL_ELECTRIC_ADC1_GPIO_Config();
	HAL_ELECTRIC_ADC1_Mode_Config();
}

/**
 * @brief ��ADֵת��Ϊ��ѹֵ
 * 
 * @param void 
 * @return uint32_t ���ص�ѹֵ
 */
uint32_t HAL_ELECTRIC_ConvertedValue(void)
{
	float Voltage;

	Voltage = HAL_ELECTRIC_ADAverageValue() * 3300.0f / 4095.0f * 4.0f; //�ⲿ��ѹΪ1/4 �ü��㹫ʽ��λΪ����

	return (uint32_t)Voltage;
}

/**
 * @brief ADC1 IO���� PA0 
 * 
 * @param void 
 */
static void HAL_ELECTRIC_ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAx, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_ADCx, ENABLE);
	RCC_ADCCLKConfig(ADC_PCLKx_Divx);

	//AIN_BATTERY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief ADC1���� ADC1-DMA1 DMA1_IN1 ADC1_IN0
 * 
 * @param void 
 */
static void HAL_ELECTRIC_ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA channel1 configuration */
	DMA_DeInit(DMAx_Channelx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_pu16ADC_ConverValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = CONVERTED_VALUE_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMAx_Channelx, &DMA_InitStructure);

	/* Enable DMA channel1 */
	DMA_Cmd(DMAx_Channelx, ENABLE);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//adc����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//ɨ��ģʽ��ʹ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					//��������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADCx, &ADC_InitStructure);

	/*����ADC1 ͨ��0 5����������  ���ù���ͨ�� */
	ADC_RegularChannelConfig(ADCx, ADC_Channelx, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */ //ʹ��ADC DMA
	ADC_DMACmd(ADCx, ENABLE);

	/* Enable ADC1 */ //ʹ��ADC
	ADC_Cmd(ADCx, ENABLE);

	/* Enable ADC1 reset calibaration register */ //ִ�и�λУ׼
	ADC_ResetCalibration(ADCx);
	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADCx)); //�ȴ�У׼�Ĵ�����λ

	/* Start ADC1 calibaration */ //��ʼִ��У׼
	ADC_StartCalibration(ADCx);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADCx)); //�ȴ�У׼���

	/* Start ADC1 Software Conversion */ //�������ת��
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

/**
 * @brief AD�ɼ�ȡƽ��ֵ
 * 
 * @param void 
 * @return uint16_t adֵ
 */
uint16_t HAL_ELECTRIC_ADAverageValue(void) //AD软件滤波
{
	uint8_t i;
	uint32_t sum = 0;

	PUB_LIB_QuickSort(g_pu16ADC_ConverValue, CONVERTED_VALUE_LEN);		//���� �Ӵ�С����

	for (i = 1; i < CONVERTED_VALUE_LEN - 1; i++)						//AD����˲� �����ֵ����Сֵȥ�� ȡʣ��ֵ��ƽ��ֵ
	{
		sum += g_pu16ADC_ConverValue[i];
	}
	return (sum / (CONVERTED_VALUE_LEN - 2));
}
