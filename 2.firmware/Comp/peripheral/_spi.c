/*
 * _spi.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: jojae
 */


#include "_spi.h"

extern SPI_HandleTypeDef hspi1;

spi_tbl_t spi_tbl[4] = {
    {},

};

static void NSS_L(uint8_t ch){ HAL_GPIO_WritePin(spi_tbl[ch].nss_port, spi_tbl[ch].nss_pin, GPIO_PIN_RESET); }
static void NSS_H(uint8_t ch){ HAL_GPIO_WritePin(spi_tbl[ch].nss_port, spi_tbl[ch].nss_pin, GPIO_PIN_SET); }

void spiInit(void)
{

}

void spiMain(void)
{

}

//TODO 아직 정리 안함
//HAL_StatusTypeDef rfidSpiTransmit(uint8_t ch, uint8_t address, uint8_t* pdata, uint8_t len)
//{
//  if(len == 0) return HAL_ERROR;
//  uint8_t send_buffer[64];
//  memset(send_buffer, 0, 64);
//
//  NSS_L(ch);
//  HAL_StatusTypeDef status = HAL_OK;
//  uint8_t wr_addr = st25_cmd_wr(address);
//
//  send_buffer[0] = wr_addr;
//  memcpy(&send_buffer[1],pdata,len);
////  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, &wr_addr, 1, 100);
////  if(status == HAL_OK) status = HAL_SPI_Transmit(&hspi1, pdata, len, 500);
//
//  status = HAL_SPI_Transmit(spi_tbl[ch].spi_handler, send_buffer, len + 1, 500);
//  NSS_H(ch);
//
//  return status;
//}
//
//HAL_StatusTypeDef rfidSpiReceive(uint8_t ch,uint8_t address, uint8_t* pdata, uint8_t len)
//{
//  if(len == 0) return HAL_ERROR;
//  NSS_L(ch);
//  HAL_StatusTypeDef status = HAL_OK;
//  uint8_t rd_addr = st25_cmd_wr(address);
//  status = HAL_SPI_Transmit(spi_tbl[ch].spi_handler, &rd_addr, 1, 100);
//  if(status == HAL_OK) status = HAL_SPI_Receive(spi_tbl[ch].spi_handler, pdata, len, 500);
//  NSS_H(ch);
//  return status;
//}
