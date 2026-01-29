/*
 * _spi.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: jojae
 */


#include "_spi.h"

#define DEFAULT_SPI_TIMEOUT 500

extern SPI_HandleTypeDef hspi3;

spi_tbl_t spi_tbl[HW_SPI_MAX_CH] = {
    {.spi_handler = &hspi3, .nss_port = SPI3_NSS_GPIO_Port, .nss_pin = SPI3_NSS_Pin},

};

static void NSS_L(uint8_t ch){ HAL_GPIO_WritePin(spi_tbl[ch].nss_port, spi_tbl[ch].nss_pin, GPIO_PIN_RESET); }
static void NSS_H(uint8_t ch){ HAL_GPIO_WritePin(spi_tbl[ch].nss_port, spi_tbl[ch].nss_pin, GPIO_PIN_SET); }

void spiInit(void)
{
  return;
}

void spiMain(void)
{
  return;
}

HAL_StatusTypeDef spiTransmit(uint8_t ch, uint8_t *pdata, uint8_t len)
{
  HAL_StatusTypeDef ret = HAL_OK;
  if(len < 0) return HAL_ERROR;

  NSS_L(ch);
  ret = HAL_SPI_Transmit(spi_tbl[ch].spi_handler, pdata, len, DEFAULT_SPI_TIMEOUT);
  NSS_H(ch);

  return ret;
}

HAL_StatusTypeDef spiReceive(uint8_t ch, uint8_t *pdata, uint8_t len)
{
  HAL_StatusTypeDef ret = HAL_OK;
  if(len < 0) return HAL_ERROR;

  NSS_L(ch);
  ret = HAL_SPI_Receive(spi_tbl[ch].spi_handler, pdata, len, DEFAULT_SPI_TIMEOUT);
  NSS_H(ch);

  return ret;
}

HAL_StatusTypeDef spiTransmitReceive(uint8_t ch, uint8_t *send_pdata, uint8_t *get_pdata, uint8_t len)
{
  HAL_StatusTypeDef ret = HAL_OK;
  if(len < 0) return HAL_ERROR;

  NSS_L(ch);
  ret = HAL_SPI_TransmitReceive(spi_tbl[ch].spi_handler, send_pdata, get_pdata, len, DEFAULT_SPI_TIMEOUT);
  NSS_H(ch);
  return ret;
}
