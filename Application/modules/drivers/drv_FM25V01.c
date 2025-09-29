//#include "../../../Application/include/drivers/drv_FM25V01.h"
//
//
//
//// Function to enable write operations
//void FRAM_WriteEnable(void)
//{
//  uint8_t command = FRAM_WRITE_ENABLE;
//
//  FRAM_CS_LOW();
//
//  HAL_SPI_Transmit(FRAM_SPI, &command, 1, HAL_MAX_DELAY);
//
//  FRAM_CS_HIGH();
//}
//
//// Function to disable write operations
//void FRAM_WriteDisable(void)
//{
//  uint8_t command = FRAM_WRITE_DISABLE;
//
//  FRAM_CS_LOW();
//
//  HAL_SPI_Transmit(FRAM_SPI, &command, 1, HAL_MAX_DELAY);
//
//  FRAM_CS_HIGH();
//}
//
//// Function to read the status register
//uint8_t FRAM_ReadStatus(void)
//{
//  uint8_t command = FRAM_READ_STATUS;
//  uint8_t status;
//
//  FRAM_CS_LOW();
//
//  HAL_SPI_Transmit(FRAM_SPI, &command, 1, HAL_MAX_DELAY);
//  HAL_SPI_Receive(FRAM_SPI, &status, 1, HAL_MAX_DELAY);
//
//  FRAM_CS_HIGH();
//
//  return status;
//}
//
//// Function to write the status register
//void FRAM_WriteStatus(uint8_t status)
//{
//  uint8_t command[2] = {FRAM_WRITE_STATUS, status};
//
//  FRAM_CS_LOW();
//
//  HAL_SPI_Transmit(FRAM_SPI, command, 2, HAL_MAX_DELAY);
//
//  FRAM_CS_HIGH();
//}
//
//
//
//
//// Function to read data from a specified address
//void FRAM_ReadData(uint16_t address, uint8_t *data, uint16_t length)
//{
//  uint8_t command[3] = {FRAM_READ, (uint8_t)(address >> 8), (uint8_t)address};
//
//  FRAM_CS_LOW();
//
//  HAL_SPI_Transmit(FRAM_SPI, command, 3, HAL_MAX_DELAY);
//  HAL_SPI_Receive(FRAM_SPI, data, length, HAL_MAX_DELAY);
//
//  FRAM_CS_HIGH();
//}
//
//// Function to write data to a specified address
//void FRAM_WriteData(uint16_t address, uint8_t *data, uint16_t length)
//{
//  FRAM_WriteEnable(); // Enable write operations
//  uint8_t command[3] = {FRAM_WRITE, (uint8_t)(address >> 8), (uint8_t)address};
//
//  FRAM_CS_LOW();
//
//  HAL_SPI_Transmit(FRAM_SPI, command, 3, HAL_MAX_DELAY);
//  HAL_SPI_Transmit(FRAM_SPI, data, length, HAL_MAX_DELAY);
//
//  FRAM_CS_HIGH();
//
//  FRAM_WriteDisable(); // Disable write operations
//}
//
