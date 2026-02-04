/*
 * bno085_spi.c
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#include "bno085_spi.h"

#ifdef BNO_SPI

//인터럽트
bool intOk = false;
static void enableInt(void) {intOk = true;}
static void disableInt(void){intOk = false;}

bool inReset = false;
BNO_SPI_SEQ bno_seq = SPI_INIT;

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
  disableInt();
  RSTN(false);
  NSS(false);
  delay(10);
  //SPI 모드
  PS0_wake(true);
  PS1(true);
  RSTN(true);


  inReset = true;

  //SPI SCK 동기화
  bno_seq = SPI_DUMMY;
  bno085DummyOp();
  bno_seq = SPI_IDLE;

  delay(10);
  RSTN(true);

  enableInt();
  delay(2000);
}



bool intTrigger = false;
bool spiTrigger = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(intOk && GPIO_Pin == BNO085_INT_Pin)
  {
    intTrigger = true;
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(intOk && hspi == &BNO_SPI_HANDLER)
  {
    spiTrigger  = true;
  }
}

bool extiIntCheck(bool reset)
{
  bool ret = intTrigger;
  if(reset)
    intTrigger = false;
  return ret;
}

bool waitExtiIntCheck(bool reset)
{
  bool ret = intTrigger;

  uint32_t timer = millis();
  while((millis() - timer) < BNO_DEFAULT_WAIT_TIME)
  {
    if(intTrigger == true)
    {
      if(reset)
        intTrigger = false;
      return true;
    }
    delay(1);
  }
  return ret;
}

#endif
