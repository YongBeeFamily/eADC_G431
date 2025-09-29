///*
// * drv_ad7799.c
// *
// *  Created on: Dec 15, 2023
// *      Author: kjkim
// */
//
//
//
//#define DATA_DRV_AD7799
//
///* Includes ------------------------------------------------------------------*/
//#include "../../../Application/include/drivers/drv_ad7799.h"
///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
///* Private function prototypes -----------------------------------------------*/
///* Exported functions --------------------------------------------------------*/
///* Private functions ---------------------------------------------------------*/
//
//extern SPI_HandleTypeDef hspi1;
//
//void SPI_Write(SPI_TypeDef* SPIx ,uint8_t *data, uint16_t size)
//{
////  for(int i=0;i<size;i++)
//// {
////    TM_SPI_Send(SPIx, data[i]);
////  }
//
////  HAL_SPI_Transmit(SPIx, data, size, 0xff);
//  HAL_SPI_Transmit(&hspi1, data, size, 10);
//}
//
//void CS_select(SPI_TypeDef* SPIx ,GPIO_PinState Output)
//{
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, Output);
//}
//
//void SPI_Read(SPI_TypeDef* SPIx ,uint8_t *data, uint16_t size)
//{
////  for(int i=0;i<size;i++)
////  {
////    data[i]= TM_SPI_Send(SPIx, 0xFF);
////  }
//
////  HAL_SPI_Receive(SPIx, data, size, 0xff);
////  HAL_SPI_Receive_DMA(&hspi1, data, size);
//  HAL_SPI_TransmitReceive(&hspi1, 0xff, data, size, 10);
//}
//
//void AD7799_Reset(SPI_TypeDef* SPIx)
//{
//  uint8_t dataToSend[5] = {0x03, 0xff, 0xff, 0xff, 0xff };
//
//  AD7799_CS_LOW();
////  SPI_Write(SPIx,dataToSend,4);
////  HAL_SPI_Transmit_DMA(&hspi1, dataToSend, 4);
//  HAL_SPI_Transmit(&hspi1, dataToSend, 4, 10);
//
//  AD7799_CS_HIGH();
//}
//
//void AD7799_SetRegisterValue(SPI_TypeDef* SPIx, uint8_t regAddress, uint32_t regValue, uint8_t size)
//{
//  uint8_t data[5] = { 0x03, 0x00, 0x00, 0x00, 0x00  };
//  data[1] = AD7799_COMM_WRITE |  AD7799_COMM_ADDR(regAddress);
//  if(size == 1)
//  {
//    data[2] = (uint8_t)regValue;
//  }
//  if(size == 2)
//  {
//    data[3] = (uint8_t)((regValue & 0x0000FF) >> 0);
//    data[2] = (uint8_t)((regValue & 0x00FF00) >> 8);
//  }
//  if(size == 3)
//  {
//    data[4] = (uint8_t)((regValue & 0x0000FF) >> 0);
//    data[3] = (uint8_t)((regValue & 0x00FF00) >> 8);
//    data[2] = (uint8_t)((regValue & 0xFF0000) >> 16);
//  }
//  AD7799_CS_LOW();      //AD7799_CS_LOW;
//
//  //SPI_Write(SPIx,&data[1],(size + 1));
////  HAL_SPI_Transmit_DMA(&hspi1, data[1], (size+1));
//  HAL_SPI_Transmit(&hspi1, data[1], (size+1), 10);
//
//  AD7799_CS_HIGH();        //AD7799_CS_HIGH;
//
//}
//
//uint8_t data[5];
//uint32_t AD7799_GetRegisterValue(SPI_TypeDef* SPIx, uint8_t regAddress, uint8_t size)
//{
////  uint8_t data[5] = { 0x03, 0x00, 0x00, 0x00, 0x00 };
//	data[0] = 0x03;
//
//  uint8_t temp;
//  uint32_t receivedData = 0x00;
//  data[1] = AD7799_COMM_READ |  AD7799_COMM_ADDR(regAddress);
//  /*
//  CS_select(SPIx, GPIO_PIN_RESET);      //AD7799_CS_LOW;
//  SPI_Write(SPIx,&data[1],1);
//  SPI_Read(SPIx,data,size);
//  CS_select(SPIx, GPIO_PIN_SET);        //AD7799_CS_HIGH;
//  */
//
//  AD7799_CS_LOW();
//
////  HAL_SPI_Transmit(&hspi1, data[1], 1, 0xff);
////  HAL_SPI_Receive(&hspi1, data, size, 0xff);
//
////  HAL_SPI_TransmitReceive_DMA(&hspi1, data[1], data, size);
//  HAL_SPI_TransmitReceive(&hspi1, data[1], data, size, 10);
//
////  HAL_SPI_Transmit_DMA(&hspi1, data[1], 1);
////  HAL_SPI_Receive_DMA(&hspi1, data, size);
//
//  AD7799_CS_HIGH();
//
//  if(size == 1)
//  {
//    receivedData += (data[0] << 0);
//  }
//  if(size == 2)
//  {
//    receivedData += (data[0] << 8);
//    receivedData += (data[1] << 0);
//  }
//  if(size == 3)
//  {
//    receivedData += (data[0] << 16);
//    receivedData += (data[1] << 8);
//    receivedData += (data[2] << 0);
//  }
//  return receivedData;
//}
//
//uint8_t status = 0x1;
//uint8_t AD7799_Init(SPI_TypeDef* SPIx){
//
//  status = AD7799_GetRegisterValue(SPIx, AD7799_REG_ID, 1);
//
//  if((status & 0x0F) != AD7799_ID){
// //   status = 0x0;
//  }
//  return(status);
//}
//
//uint8_t AD7799_Ready(SPI_TypeDef* SPIx)
//{
//  uint8_t rdy = 0;
//  rdy = (AD7799_GetRegisterValue(SPIx, AD7799_REG_STAT,1) & 0x80);
//
//  return(!rdy);
//}
