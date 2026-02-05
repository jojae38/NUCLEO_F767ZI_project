/*
 * bno085_spi.c
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#include "bno085_spi.h"

#ifdef BNO_SPI

#define READ_LEN 4
static const uint8_t txZeros[BNO_RX_BUFFER_LIMIT] = {0};

//인터럽트
bool isInit = false;
bool intOk = false;
bool inReset = false;
bool rxReady = false;

bool intTrigger = false;
uint32_t intTriggerCount = 0;

bool spiTrigger = false;
uint32_t spiTriggerCount = 0;

uint8_t rxBuf[BNO_RX_BUFFER_LIMIT];
static volatile uint32_t rxBufLen;
static volatile bool rxDataReady;

uint8_t txBuf[BNO_TX_BUFFER_LIMIT];
static uint32_t txBufLen;

static void enableInt(void) {intOk = true;}
static void disableInt(void){intOk = false;}
void spiActivate(void);
void spiComplete(void);

BNO_SPI_SEQ bno_seq = SPI_INIT;

static void NSS(bool state){HAL_GPIO_WritePin(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
static void bno085DummyOp(void);
bool bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t s_len, uint8_t* r_pdata, uint32_t r_len);
void spiRdHdr(void);
void spiRdBody(void);
void spiWrite(void);

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

void bnoSpiReset(void)
{
  disableInt();
  RSTN(false);
  NSS(false);
  delay(10);
  //SPI 모드
  PS0_wake(true);
  PS1(true);
  RSTN(true);

  rxBufLen = 0;
  txBufLen = 0;
  rxDataReady = false;
  rxReady = false;

  inReset = true;
  cliPrintf("RESET START\r\n");

  //SPI SCK 동기화
  bno_seq = SPI_DUMMY;
  bno085DummyOp();
  bno_seq = SPI_IDLE;

  delay(10);
  RSTN(true);

  enableInt();
  delay(2000);
  cliPrintf("RESET END\r\n");
  isInit = true;
}

void bnoSpiSeq(void)
{
  if(isInit == false)
    return;

  switch (bno_seq) {
    case SPI_IDLE:
      spiActivate();
      break;
    case SPI_RD_HDR_WAIT:
      break;
    case SPI_RD_HDR:
      spiRdHdr();
      break;
    case SPI_RD_BODY_WAIT:
      break;
    case SPI_RD_BODY:
      spiRdBody();
      break;
    case SPI_WRITE_WAIT:
      break;
    case SPI_WRITE:
      spiWrite();
      break;
    default:
      break;
  }
}

//SPI_IDLE
void spiActivate(void)
{
  if(rxReady && (rxBufLen == 0))
  {
    rxReady = false;
    NSS(false);

    if(txBufLen > 0)
    {
      bno_seq = SPI_WRITE_WAIT;
      HAL_SPI_TransmitReceive_IT(&BNO_SPI_HANDLER, txBuf, rxBuf, txBufLen);
        // Deassert Wake
      PS0_wake(true);
    }
    else
    {
      bno_seq = SPI_RD_HDR_WAIT;
      HAL_SPI_TransmitReceive_IT(&BNO_SPI_HANDLER, (uint8_t *)txZeros, rxBuf, READ_LEN);
    }
  }
}

void spiRdHdr(void)
{
  uint16_t rxLen = (rxBuf[0] + (rxBuf[1] << 8)) & ~0x8000;

  if (rxLen > sizeof(rxBuf))
  {
      rxLen = sizeof(rxBuf);
  }

  if (rxLen > READ_LEN) {
      // There is more to read

      // Transition to RD_BODY state
      bno_seq = SPI_RD_BODY_WAIT;

      // Start a read operation for the remaining length.  (We already read the first READ_LEN bytes.)
      HAL_SPI_TransmitReceive_IT(&BNO_SPI_HANDLER, (uint8_t *)txZeros, rxBuf+READ_LEN, rxLen-READ_LEN);
  }
  else
  {
      // No SHTP payload was received, this operation is done
      NSS(true);            // deassert CSN
      rxBufLen = 0;         // no rx data available
      bno_seq = SPI_IDLE;  // back to idle state
      spiActivate();        // activate next operation, if any.
  }
}

void spiRdBody(void)
{
  uint16_t rxLen = (rxBuf[0] + (rxBuf[1] << 8)) & ~0x8000;

  if (rxLen > sizeof(rxBuf))
  {
      rxLen = sizeof(rxBuf);
  }

  // We completed the read or write of a payload
  // deassert CSN.
  NSS(true);

  // Check len of data read and set rxBufLen
  rxBufLen = rxLen;

  // transition back to idle state
  bno_seq = SPI_IDLE;
}

void spiWrite(void)
{
  uint16_t rxLen = (rxBuf[0] + (rxBuf[1] << 8)) & ~0x8000;

  if (rxLen > sizeof(rxBuf))
  {
      rxLen = sizeof(rxBuf);
  }

  // We completed the read or write of a payload
  // deassert CSN.
  NSS(true);

  // Since operation was a write, transaction was for txBufLen bytes.  So received
  // data len is, at a maximum, txBufLen.
  rxBufLen = (txBufLen < rxLen) ? txBufLen : rxLen;

  // Tx buffer is empty now.
  txBufLen = 0;

  // transition back to idle state
  bno_seq = SPI_IDLE;
}

//void spiComplete(void)
//{
//  uint16_t rxLen = (rxBuf[0] + (rxBuf[1] << 8)) & ~0x8000;
//
//  if (rxLen > sizeof(rxBuf))
//  {
//      rxLen = sizeof(rxBuf);
//  }
//
//  if (bno_seq == SPI_DUMMY)
//  {
//      // SPI Dummy operation completed, transition now to idle
//      bno_seq = SPI_IDLE;
//  }
//  else if(bno_seq == SPI_RD_HDR)
//  {
//    if (rxLen > READ_LEN) {
//        // There is more to read
//
//        // Transition to RD_BODY state
//        bno_seq = SPI_RD_BODY;
//
//        // Start a read operation for the remaining length.  (We already read the first READ_LEN bytes.)
//        HAL_SPI_TransmitReceive_IT(&BNO_SPI_HANDLER, (uint8_t *)txZeros, rxBuf+READ_LEN, rxLen-READ_LEN);
//    }
//    else
//    {
//        // No SHTP payload was received, this operation is done
//        NSS(true);            // deassert CSN
//        rxBufLen = 0;         // no rx data available
//        bno_seq = SPI_IDLE;  // back to idle state
//        spiActivate();        // activate next operation, if any.
//    }
//  }
//  else if (bno_seq == SPI_RD_BODY)
//  {
//      // We completed the read or write of a payload
//      // deassert CSN.
//      NSS(true);
//
//      // Check len of data read and set rxBufLen
//      rxBufLen = rxLen;
//
//      // transition back to idle state
//      bno_seq = SPI_IDLE;
//
//      // Activate the next operation, if any.
//      spiActivate();
//  }
//  else if (bno_seq == SPI_WRITE)
//  {
//      // We completed the read or write of a payload
//      // deassert CSN.
//      NSS(true);
//
//      // Since operation was a write, transaction was for txBufLen bytes.  So received
//      // data len is, at a maximum, txBufLen.
//      rxBufLen = (txBufLen < rxLen) ? txBufLen : rxLen;
//
//      // Tx buffer is empty now.
//      txBufLen = 0;
//
//      // transition back to idle state
//      bno_seq = SPI_IDLE;
//
//      // Activate the next operation, if any.
//      spiActivate();
//  }
//}

void extiInterrupt(void)
{
  intTriggerCount ++;
  inReset = false;
  rxReady = true;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(intOk && GPIO_Pin == BNO085_INT_Pin)
  {
    extiInterrupt();
  }
}

void spiInterrupt(void)
{
  spiTriggerCount ++;
  switch (bno_seq) {
    case SPI_DUMMY:
      bno_seq = SPI_INIT;
      break;
    case SPI_RD_HDR_WAIT:
      bno_seq = SPI_RD_HDR;
      break;
    case SPI_RD_BODY_WAIT:
      bno_seq = SPI_RD_BODY;
      break;
    case SPI_WRITE_WAIT:
      bno_seq = SPI_WRITE;
      break;
    default:
      break;
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(intOk && hspi == &BNO_SPI_HANDLER)
  {
//    spiComplete();
    spiInterrupt();
  }
}

#endif
