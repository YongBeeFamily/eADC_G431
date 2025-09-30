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
extern str_bit CBIT, IBIT, PBIT;

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
//    HAL_FLASH_Unlock();
//    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data);
//    HAL_FLASH_Lock();

	uint8_t *pData = (uint8_t *)&AppSettings;
	HAL_FLASH_Unlock();
	for (uint32_t i = 0; i < sizeof(APP_SETTINGS); i += 8) // G4는 Double Word(8바이트) 단위 쓰기
	{
	    uint64_t data64 = 0;
	    memcpy(&data64, pData + i, sizeof(uint64_t));
	    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, USER_DATA_ADDR + i, data64);
	}
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
			memcpy((void *)&RxCali, (void *)&RxBuf, sizeof(RxCali));

			checksum = 0;
			for (int i = 2; i < sizeof(RxCali); i++)
			{
				checksum ^= RxBuf[i];
			}
			if (checksum == 0)
			{
				tempFloat = (float)RxCali.data[0];
				AppSettings.CORRECTIONPRESSUREVALUE = sensor[0].sensor_data.pressure - (tempFloat*100);

				Flash_Erase_Page(USER_DATA_ADDR);
				Flash_Write(USER_DATA_ADDR, (uint64_t)&AppSettings);
			}
		}
		else if ((RxBuf[0] == 'L') && (RxBuf[1] == 'H'))
		{
			memcpy((void *)&RxCali, (void *)&RxBuf, sizeof(RxCali));

			checksum = 0;
			for (int i = 2; i < sizeof(RxCali); i++)
			{
				checksum ^= RxBuf[i];
			}

			if (checksum == 0)
			{
				memset((void *)AppSettings.SERIALNO, 0, sizeof(AppSettings.SERIALNO));
				strcpy((char *)AppSettings.SERIALNO, (char *)RxCali.SERIALNO);

				Flash_Erase_Page(USER_DATA_ADDR);
				Flash_Write(USER_DATA_ADDR, (uint64_t)&AppSettings);
			}
		}

		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuf, sizeof(RxBuf));
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}

uint8_t tempBuf[100];

void task_Flash(void const * argument)
{
	/* USER CODE BEGIN task_GCS2ADS */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = 100;

//	Flash_Erase_Page(USER_DATA_ADDR);
//	Flash_Write(USER_DATA_ADDR, DEFAULT_CORRECTIONPRESSUREVALUE);
//	osDelay(1000);
//	tempFloatValue = Flash_Read(USER_DATA_ADDR);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuf, sizeof(RxBuf));
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	uint8_t *pRead = (uint8_t *)&AppSettings;
	for (uint32_t i = 0; i < sizeof(APP_SETTINGS); i++)
	{
	    pRead[i] = *(uint8_t *)(USER_DATA_ADDR + i);
	}


	for(;;)
	{

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}



