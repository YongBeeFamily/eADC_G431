/*
 * task_Flash.c
 *
 *  Created on: Aug 6, 2025
 *      Author: tocru
 */
#include "task_Flash.h"
#include "cmsis_os.h"
#include "app_config.h"
#include "module_bmp581.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern BMP581_DEV sensor[4];

Str_uart_rx RxCali;
APP_SETTINGS AppSettings;
uint8_t RxBuf[100] = {0,};


uint64_t Float2uint64_t(float fData)
{
	union {
		uint64_t		x;
		float			f[2];
	} z;

	z.f[0] = fData;
	z.f[1] = 0;
	return z.x;
}

float uint64_t2Float(uint64_t data)
{
	union {
		uint64_t		x;
		uint8_t			y[8];
		float			f[2];
	} z;

	z.x = data;
	return z.f[0];
}


void Flash_Write(uint32_t address, uint64_t data) {
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data);
    HAL_FLASH_Lock();
}

uint8_t Flash_Read(uint32_t address) {
    return *(uint8_t*)address;
}

void Flash_Erase_Page(uint32_t pageAddress) {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page      = (pageAddress - FLASH_BASE) / FLASH_PAGE_SIZE;
    EraseInitStruct.NbPages   = 1;

    HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

    HAL_FLASH_Lock();
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint8_t checksum = 0;
	float tempFloat;
	uint64_t temp64;

	if  (huart->Instance == USART1)
	{
		if ((RxBuf[0] == 'L') && (RxBuf[1] == 'G'))
		{
			memcpy((void *)&RxCali, (void *)&RxBuf, sizeof(RxBuf));

			checksum = 0;
			for (int i = 2; i < sizeof(RxCali)-1; i++)
			{
				checksum ^= RxBuf[i];
			}

			if (checksum == 0)
			{
				AppSettings.CORRECTIONPRESSUREVALUE = sensor[0].sensor_data.pressure - (tempFloat*100);
//				temp64 = Float2uint64_t(AppSettings.CORRECTIONPRESSUREVALUE);

				Flash_Erase_Page(USER_DATA_ADDR);
				Flash_Write(USER_DATA_ADDR, (uint64_t)&AppSettings);
			}
		} else if ((RxBuf[0] == 'L') && (RxBuf[1] == 'H'))
		{
			memcpy((void *)&RxCali, (void *)&RxBuf, sizeof(RxBuf));

			checksum = 0;
			for (int i = 2; i < sizeof(RxCali)-1; i++)
			{
				checksum ^= RxBuf[i];
			}

			if (checksum == 0)
			{
				strcpy((char *)AppSettings.SERIALNO, (char *)RxCali.SERIALNO);
				tempFloat = RxCali.data[0];

				Flash_Erase_Page(USER_DATA_ADDR);
				Flash_Write(USER_DATA_ADDR, (uint64_t)&AppSettings);
			}
		}

		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuf, sizeof(RxBuf));
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}



void task_Flash(void const * argument)
{
	/* USER CODE BEGIN task_GCS2ADS */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = 100;
	uint8_t* temp;

	temp = malloc(sizeof(APP_SETTINGS));

//	Flash_Erase_Page(USER_DATA_ADDR);
//	Flash_Write(USER_DATA_ADDR, DEFAULT_CORRECTIONPRESSUREVALUE);
//	osDelay(1000);
//	tempFloatValue = Flash_Read(USER_DATA_ADDR);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuf, sizeof(RxBuf));
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	AppSettings.CORRECTIONPRESSUREVALUE = DEFAULT_CORRECTIONPRESSUREVALUE;

	temp = Flash_Read(USER_DATA_ADDR);
	AppSettings = *(APP_SETTINGS *)temp;

	for(;;)
	{

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}



