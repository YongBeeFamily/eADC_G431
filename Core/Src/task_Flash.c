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
#include "stdio.h"

#include <string.h>


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern BMP581_DEV sensor[4];

void ReturnAppSettings(void);

Str_uart_rx RxCali;
APP_SETTINGS AppSettings;
uint8_t RxBuf[100] = {0,};
extern str_bit CBIT, IBIT, PBIT;

void Flash_ReadBuffer(uint32_t address, void *buffer, uint32_t size);

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

// Write a buffer to flash in double-word (8-byte) units and verify the write.
void Flash_WriteBuffer(uint32_t address, const void *buffer, uint32_t size)
{
    const uint8_t *pData = (const uint8_t *)buffer;
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < size; i += 8) // G4는 Double Word(8바이트) 단위 쓰기
    {
        uint64_t data64 = 0;
        uint32_t remain = size - i;
        if (remain >= 8) {
            memcpy(&data64, pData + i, sizeof(uint64_t));
        } else {
            uint8_t tmp[8];
            for (uint32_t k = 0; k < 8; k++) tmp[k] = 0xFF; // erased flash is 0xFF
            memcpy(tmp, pData + i, remain);
            memcpy(&data64, tmp, sizeof(uint64_t));
        }
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + i, data64) != HAL_OK) {
            // failed to program; stop further writes
            break;
        }
    }
    HAL_FLASH_Lock();

    // Verify (only supports sizes up to APP_SETTINGS for simplicity)
    if (size <= sizeof(APP_SETTINGS)) {
        uint8_t tmp[sizeof(APP_SETTINGS)];
        Flash_ReadBuffer(address, tmp, size);
        if (memcmp(tmp, buffer, size) != 0) {
            // verification failed - you could set an error flag or retry
        }
    }
}

// new helper: read buffer from flash
void Flash_ReadBuffer(uint32_t address, void *buffer, uint32_t size)
{
	uint8_t *pDst = (uint8_t *)buffer;
	for (uint32_t i = 0; i < size; i++) {
		pDst[i] = *(uint8_t *)(address + i);
	}
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

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
        // optionally handle erase error
    }

    HAL_FLASH_Lock();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint8_t checksum = 0;
	float tempFloat, tempFloat2;
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
				tempFloat2 = (float)RxCali.data[1];
//				if (tempFloat < 1.0)
//				{
//					tempFloat = 1013.25f;
//					AppSettings.CORRECTIONPRESSUREVALUE = sensor[0].sensor_data.pressure - (tempFloat*100);
//				}
//				else
//				{
//					AppSettings.CORRECTIONPRESSUREVALUE = tempFloat;
//				}
				AppSettings.CORRECTIONPRESSUREVALUE = tempFloat;
				AppSettings.CORRECTIONPRESSUREVALUE2 = tempFloat2;

				Flash_Erase_Page(USER_DATA_ADDR);
				// write the whole AppSettings structure to flash
				Flash_WriteBuffer(USER_DATA_ADDR, &AppSettings, sizeof(AppSettings));
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
				if (RxCali.SERIALNO[0] != 0)
				{
					memset((void *)AppSettings.SERIALNO, 0, sizeof(AppSettings.SERIALNO));
					strcpy((char *)AppSettings.SERIALNO, (char *)RxCali.SERIALNO);
				}

				Flash_Erase_Page(USER_DATA_ADDR);
				Flash_WriteBuffer(USER_DATA_ADDR, &AppSettings, sizeof(AppSettings));
			}
		}

		ReturnAppSettings();
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuf, sizeof(RxBuf));
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}


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

	// Read saved settings from flash into AppSettings
	Flash_ReadBuffer(USER_DATA_ADDR, (void *)&AppSettings, sizeof(AppSettings));

	// Validate signature; if invalid, initialize defaults and write to flash
	if (AppSettings.signature != APP_SETTINGS_MAGIC) {
		// initialize default settings
		memset(&AppSettings, 0x00, sizeof(AppSettings));
		AppSettings.signature = APP_SETTINGS_MAGIC;
		AppSettings.CORRECTIONPRESSUREVALUE = DEFAULT_CORRECTIONPRESSUREVALUE;
		AppSettings.CORRECTIONPRESSUREVALUE2 = DEFAULT_CORRECTIONPRESSUREVALUE;
		// optional: default serial blank
		memset(AppSettings.SERIALNO, 0, sizeof(AppSettings.SERIALNO));

		// persist defaults
		Flash_Erase_Page(USER_DATA_ADDR);
		Flash_WriteBuffer(USER_DATA_ADDR, &AppSettings, sizeof(AppSettings));
	}

	osDelay(2000);
	ReturnAppSettings();

	for(;;)
	{

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
uint8_t tempBuf[100] = {0,};
void ReturnAppSettings(void)
{

	uint8_t Checksum = 0;

	memset((void*)&RxCali, 0, sizeof(RxCali));
	RxCali.HEADER1 = HEADER_ADS2OFP2;
	RxCali.HEADER2 = HEADER_ADS2OFP3;

	memcpy((void*)&RxCali.data[0], (void*)&AppSettings.CORRECTIONPRESSUREVALUE, sizeof(float));
	memcpy((void*)&RxCali.data[1], (void*)&AppSettings.CORRECTIONPRESSUREVALUE2, sizeof(float));
	memcpy((void*)&RxCali.SERIALNO[0], (void*)&AppSettings.SERIALNO[0], sizeof(AppSettings.SERIALNO));

	memcpy(&tempBuf[0], & RxCali, sizeof(RxCali));

	for(int i = 2; i < sizeof(RxCali)-1; i++)
	{
		Checksum ^=  tempBuf[i];
	}
	tempBuf[sizeof(RxCali) - 1] = Checksum;

	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&tempBuf, sizeof(RxCali));
}
