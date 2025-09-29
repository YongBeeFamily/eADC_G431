/**
 * Copyright (C) 2022 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "../../../Application/include/drivers/bmp5_defs.h"

/******************************************************************************/
/*!                         Macro definitions                                 */

/*! BMP5 shuttle id */
#define BMP5_SHUTTLE_ID_PRIM  UINT16_C(0x1B3)
#define BMP5_SHUTTLE_ID_SEC   UINT16_C(0x1D3)

/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr;

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * I2C read function map to COINES platform
 */
BMP5_INTF_RET_TYPE bmp5_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
	BMP581_DEV dev = *(BMP581_DEV*)intf_ptr;
    uint8_t device_addr = dev.dev_addr;
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)dev.intf_ptr;

//    return coines_read_i2c(COINES_I2C_BUS_0, device_addr, reg_addr, reg_data, (uint16_t)length);
//    return HAL_I2C_Mem_Read(&hi2c1, device_addr, reg_addr, 1, reg_data, length, 0xff);
    return HAL_I2C_Mem_Read(hi2c, device_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, length, 0xff);
}

/*!
 * I2C write function map to COINES platform
 */
BMP5_INTF_RET_TYPE bmp5_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
	BMP581_DEV dev = *(BMP581_DEV*)intf_ptr;
    uint8_t device_addr = dev.dev_addr;
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)dev.intf_ptr;

//    return coines_write_i2c(COINES_I2C_BUS_0, device_addr, reg_addr, (uint8_t *)reg_data, (uint16_t)length);
    return HAL_I2C_Mem_Write(hi2c, device_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, length, 0xff);
}

/*!
 * SPI read function map to COINES platform
 */
BMP5_INTF_RET_TYPE bmp5_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    (void)intf_ptr;

//    return coines_read_spi(COINES_SPI_BUS_0, device_addr, reg_addr, reg_data, (uint16_t)length);
    return 0;
}

/*!
 * SPI write function map to COINES platform
 */
BMP5_INTF_RET_TYPE bmp5_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    (void)intf_ptr;

//    return coines_write_spi(COINES_SPI_BUS_0, device_addr, reg_addr, (uint8_t *)reg_data, (uint16_t)length);
    return 0;
}

/*!
 * Delay function map to COINES platform
 */
void bmp5_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    delay_us(period);
}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void bmp5_error_codes_print_result(const char api_name[], int8_t rslt)
{
    if (rslt != BMP5_OK)
    {
        printf("%s\t", api_name);
        if (rslt == BMP5_E_NULL_PTR)
        {
            printf("Error [%d] : Null pointer\r\n", rslt);
        }
        else if (rslt == BMP5_E_COM_FAIL)
        {
            printf("Error [%d] : Communication failure\r\n", rslt);
        }
        else if (rslt == BMP5_E_DEV_NOT_FOUND)
        {
            printf("Error [%d] : Device not found\r\n", rslt);
        }
        else if (rslt == BMP5_E_INVALID_CHIP_ID)
        {
            printf("Error [%d] : Invalid chip id\r\n", rslt);
        }
        else if (rslt == BMP5_E_POWER_UP)
        {
            printf("Error [%d] : Power up error\r\n", rslt);
        }
        else if (rslt == BMP5_E_POR_SOFTRESET)
        {
            printf("Error [%d] : Power-on reset/softreset failure\r\n", rslt);
        }
        else if (rslt == BMP5_E_INVALID_POWERMODE)
        {
            printf("Error [%d] : Invalid powermode\r\n", rslt);
        }
        else
        {
            /* For more error codes refer "*_defs.h" */
            printf("Error [%d] : Unknown error code\r\n", rslt);
        }
    }
}

/*!
 *  @brief Function to select the interface between SPI and I2C.
 */
int8_t bmp5_interface_init(BMP581_DEV *bmp5_dev, uint8_t intf)
{
    int8_t rslt = BMP5_OK;

	/* Bus configuration : I2C */
	if (intf == BMP5_I2C_INTF)
	{
//		bmp5_dev->dev_addr = (BMP5_I2C_ADDR_PRIM<<1);
		bmp5_dev->read = bmp5_i2c_read;
		bmp5_dev->write = bmp5_i2c_write;
		bmp5_dev->intf = BMP5_I2C_INTF;
	}
	/* Bus configuration : SPI */
	else if (intf == BMP5_SPI_INTF)
	{
//            bmp5_dev->dev_addr = COINES_SHUTTLE_PIN_7;
		bmp5_dev->read = bmp5_spi_read;
		bmp5_dev->write = bmp5_spi_write;
		bmp5_dev->intf = BMP5_SPI_INTF;
	}

	delay_ms(10);

	/* Configure delay in microseconds */
	bmp5_dev->delay_us = bmp5_delay_us;

    return rslt;
}

void bmp5_coines_deinit(void)
{
    (void)fflush(stdout);

    (void)coines_set_shuttleboard_vdd_vddio_config(0, 0);
    coines_delay_msec(100);

    /* Coines interface reset */
    coines_soft_reset();
    coines_delay_msec(100);

//    (void)coines_close_comm_intf(COINES_COMM_INTF_USB, NULL);
}
