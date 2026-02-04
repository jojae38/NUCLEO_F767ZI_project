/*
 * bno085_int.c
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */
#include "bno085_int.h"

bool intOk = false;

void enableInt(void)
{
  intOk = true;
}

void disableInt(void)
{
  intOk = false;
}

#ifdef BNO_SPI

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

bool waitExtiInterrupt(bool reset)
{
  bool ret = intTrigger;

  uint32_t timer = millis();
  while((millis() - timer) < BNO_DEFAULT_WAIT_TIME)
  {
    if(intTrigger == true)
    {
      if(reset == true)
        intTrigger = false;
      return true;
    }
    delay(1);
  }
  return ret;
}

#endif

#ifdef BNO_I2C

#endif

#ifdef BNO_UART

#endif

#ifdef BNO_UARTRVC

#endif
