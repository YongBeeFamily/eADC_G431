/*
 * drv_ad7799.h
 *
 *  Created on: Dec 15, 2023
 *      Author: kjkim
 */

#ifndef INCLUDE_DRIVERS_DRV_AD7799_H_
#define INCLUDE_DRIVERS_DRV_AD7799_H_




/**
  ******************************************************************************
  * @file    drv_ad7799.h
  * @author  JMS
  * @version v1.0
  * @date    24-Jan-2019
  * @brief   AD7799 driver
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 YONGBEE AT </center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_AD7799_H
#define __DRV_AD7799_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Use Extern preprocessor for dev convinient --------------------------------*/
#ifdef DATA_DRV_AD7799
  #define DRV_AD7799_EXT
#else
  #define DRV_AD7799_EXT  extern
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
 //#include "drv_spi.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define AD7799_CS_LOW()   HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);        //HAL_GPIO_WritePin(SPI5_CS_GPIO_Port,SPI5_CS_Pin,GPIO_PIN_RESET);
#define AD7799_CS_HIGH()  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);          //HAL_GPIO_WritePin(SPI5_CS_GPIO_Port,SPI5_CS_Pin,GPIO_PIN_SET);


#define SPI1_CS_Pin 		GPIO_PIN_4
#define SPI1_CS_GPIO_Port 	GPIOA



/*AD7799 Registers*/
#define AD7799_REG_COMM         0 /* Communications Register(WO, 8-bit) */
#define AD7799_REG_STAT         0 /* Status Register      (RO, 8-bit) */
#define AD7799_REG_MODE         1 /* Mode Register        (RW, 16-bit */
#define AD7799_REG_CONF         2 /* Configuration Register (RW, 16-bit)*/
#define AD7799_REG_DATA         3 /* Data Register        (RO, 16-/24-bit) */
#define AD7799_REG_ID           4 /* ID Register        (RO, 8-bit) */
#define AD7799_REG_IO           5 /* IO Register        (RO, 8-bit) */
#define AD7799_REG_OFFSET       6 /* Offset Register      (RW, 24-bit */
#define AD7799_REG_FULLSALE     7 /* Full-Scale Register  (RW, 24-bit */

/* Communications Register Bit Designations (AD7799_REG_COMM) */
#define AD7799_COMM_WEN         (1 << 7)            /* Write Enable */
//#define AD7799_COMM_WRITE       (0 << 6)            /* Write Operation */
//#define AD7799_COMM_READ        (1 << 6)            /* Read Operation */
#define AD7799_COMM_ADDR(x)     (((x) & 0x7) << 3)  /* Register Address */
#define AD7799_COMM_CREAD       (1 << 2)            /* Continuous Read of Data Register */

#define AD7799_COMM_WRITE       0b00100000
#define AD7799_COMM_READ        0b00010000

/* ID Register Bit Designations (AD7799_REG_ID) */
#define AD7799_ID               0x9
#define AD7799_ID_MASK          0xF

/* Exported macro ------------------------------------------------------------*/
/* Exported structure --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
DRV_AD7799_EXT void AD7799_Reset(SPI_TypeDef* SPIx);
DRV_AD7799_EXT uint8_t AD7799_Init(SPI_TypeDef* SPIx);
DRV_AD7799_EXT uint8_t AD7799_Ready(SPI_TypeDef* SPIx);
DRV_AD7799_EXT uint32_t AD7799_GetRegisterValue(SPI_TypeDef* SPIx, uint8_t regAddress, uint8_t size);
DRV_AD7799_EXT void AD7799_SetRegisterValue(SPI_TypeDef* SPIx, uint8_t regAddress, uint32_t regValue, uint8_t size);
#ifdef __cplusplus
}
#endif
/* ---------------------------------------------------------------------------*/

#endif /* __DRV_UART_H */
/************************ (C) COPYRIGHT YONGBEE AT *****END OF FILE****/



#endif /* INCLUDE_DRIVERS_DRV_AD7799_H_ */
