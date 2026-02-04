/*
 * bno085_spi.c
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#include "bno085_spi.h"

#ifdef BNO_SPI

static void NSS(bool state){HAL_GPIO_WritePin(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
static void bno085DummyOp(void);


static void bno085DummyOp(void)
{
  uint8_t dummyTx[1];
  uint8_t dummyRx[1];
  memset(dummyTx,0xAA,sizeof(dummyTx));
  HAL_SPI_TransmitReceive(&BNO_SPI_HANDLER, dummyTx, dummyRx, sizeof(dummyTx), 10);
}


bool bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t s_len, uint8_t* r_pdata, uint32_t r_len)
{
  NSS(false);
  if(HAL_SPI_Transmit(&BNO_SPI_HANDLER, s_pdata, s_len, 500) != HAL_OK)
    goto error;
  if(HAL_SPI_Receive(&BNO_SPI_HANDLER, r_pdata, r_len, 500) != HAL_OK)
    goto error;

  NSS(true);
  return true;

error:
  NSS(true);
  return false;

}

void bno085Reset(void)
{
  RSTN(true);
  delay(100);
  RSTN(false);
  delay(10);
  RSTN(true);
}

#endif
