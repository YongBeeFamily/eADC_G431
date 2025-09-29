///*
// * drv_FRAM.h
// *
// *  Created on: Dec 11, 2023
// *      Author: kjkim
// */
//
//#ifndef INCLUDE_DRIVERS_DRV_FRAM_H_
//#define INCLUDE_DRIVERS_DRV_FRAM_H_
//
//#include "main.h"
//
//extern SPI_HandleTypeDef hspi2; // Assuming SPI2 is used
//
//#define	FRAM_SPI			&hspi2
//#define FRAM_CS_PORT		SPI2_CS_GPIO_Port
//#define FRAM_CS_PIN		    SPI2_CS_Pin
//
//
////FM25V01 - 16384 byte = 16KB
////#define FRAM_GCS_COEF_ADR	0x0000	// 512 byte
////#define FRAM_ISS_COEF_ADR	0x0200	// 1KB -
////#define FRAM_LOG_START		0x0400	// 15KB
////#define FRAM_LOG_END		0x3fff
//#define FRAM_GCS_COEF_ADR	0x0000	// 416 byte = DATA SIZE : 400bytes
//#define FRAM_ISS_COEF_ADR	0x01A0	// 600 byte = DATA SIZE : 534bytes
//#define FRAM_LOG_TIME_ADR	0x02F8	//   8 byte
//#define FRAM_LOG_START		0x0400	// 15KB
//#define FRAM_LOG_END		0x3FFF
//#define FLASH_SIZE			(FRAM_LOG_END - FRAM_LOG_START)
//
//
//#define FRAM_CS_LOW()		HAL_GPIO_WritePin(FRAM_CS_PORT, FRAM_CS_PIN ,GPIO_PIN_RESET)
//#define FRAM_CS_HIGH()		HAL_GPIO_WritePin(FRAM_CS_PORT, FRAM_CS_PIN ,GPIO_PIN_SET)
//
//
//
//// FRAM Command Definitions
//#define FRAM_WRITE_ENABLE  0x06
//#define FRAM_WRITE_DISABLE 0x04
//#define FRAM_READ_STATUS   0x05
//#define FRAM_WRITE_STATUS  0x01
//#define FRAM_READ          0x03
//#define FRAM_WRITE         0x02
//
//// Function prototypes
//void FRAM_WriteEnable(void);
//void FRAM_WriteDisable(void);
//uint8_t FRAM_ReadStatus(void);
//void FRAM_WriteStatus(uint8_t status);
//void FRAM_ReadData(uint16_t address, uint8_t *data, uint16_t length);
//void FRAM_WriteData(uint16_t address, uint8_t *data, uint16_t length);
//
//
//
//#endif /* INCLUDE_DRIVERS_DRV_FRAM_H_ */
