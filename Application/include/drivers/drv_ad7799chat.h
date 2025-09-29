/*
 * drv_as7799chat.h
 *
 *  Created on: Dec 15, 2023
 *      Author: kjkim
 */

#ifndef INCLUDE_DRIVERS_DRV_AD7799CHAT_H_
#define INCLUDE_DRIVERS_DRV_AD7799CHAT_H_


#include "main.h"

// AD7799 commands
#define AD7799_MODE_WRITE   0b00100000
#define AD7799_MODE_READ    0b00010000
#define AD7799_MODE_NORMAL  0b00000000

// AD7799 registers
#define AD7799_REG_COMM     0x00
#define AD7799_REG_STAT     0x00
#define AD7799_REG_MODE     0x01
#define AD7799_REG_CONF     0x02
#define AD7799_REG_DATA     0x03




void ad7799WriteRegister(uint8_t reg, uint8_t data);
uint8_t ad7799ReadRegister(uint8_t reg);
int32_t ad7799ReadData();



#endif /* INCLUDE_DRIVERS_DRV_AD7799CHAT_H_ */
