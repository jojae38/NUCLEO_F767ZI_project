/*
 * _spi.h
 *
 *  Created on: 2026. 1. 28.
 *      Author: jojae
 */

#ifndef PERIPHERAL__SPI_H_
#define PERIPHERAL__SPI_H_

#include "hw_def.h"

typedef struct{
  SPI_HandleTypeDef*    spi_handler;
  GPIO_TypeDef*         nss_port;
  uint16_t              nss_pin;
}spi_tbl_t;

void spiInit(void);
void spiMain(void);

HAL_StatusTypeDef spiTransmit(uint8_t ch, uint8_t *pdata, uint8_t len);
HAL_StatusTypeDef spiReceive(uint8_t ch, uint8_t *pdata, uint8_t len);
HAL_StatusTypeDef spiTransmitReceive(uint8_t ch, uint8_t *send_pdata, uint8_t *get_pdata, uint8_t len);

#endif /* PERIPHERAL__SPI_H_ */
