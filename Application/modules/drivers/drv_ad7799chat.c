///*
// * drv_ad7799chat.c
// *
// *  Created on: Dec 15, 2023
// *      Author: kjkim
// */
//#include "../../../Application/include/drivers/drv_ad7799chat.h"
//
//extern SPI_HandleTypeDef hspi1;
//
//
//// Function to send a byte over SPI
//void spiWrite(uint8_t data) {
//    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
//}
//
//// Function to read a byte over SPI
//uint8_t spiRead() {
//    uint8_t data;
//    HAL_SPI_Receive(&hspi1, &data, 1, HAL_MAX_DELAY);
//    return data;
//}
//
//// Function to write to a register on the AD7799
//void ad7799WriteRegister(uint8_t reg, uint8_t data) {
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS low
//    spiWrite(AD7799_MODE_WRITE | reg);
//    spiWrite(data);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // CS high
//}
//
//// Function to read from a register on the AD7799
//uint8_t ad7799ReadRegister(uint8_t reg) {
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS low
//    spiWrite(AD7799_MODE_READ | reg);
//    uint8_t data = spiRead();
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // CS high
//    return data;
//}
//
//
//// Function to read 24-bit data from the AD7799
//int32_t ad7799ReadData() {
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS low
//    spiWrite(AD7799_MODE_READ | AD7799_REG_DATA);
//    uint32_t data = 0;
//    data |= (spiRead() << 16);
//    data |= (spiRead() << 8);
//    data |= spiRead();
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // CS high
//
//    // Extend sign bit for 24-bit signed data
//    if (data & 0x800000) {
//        data |= 0xFF000000;
//    }
//
//    return data;
//}
//
//
//
//
