/*
 * bno085.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "bno085.h"
#include "_spi.h"
#include "cli.h"

bno085_setting_tbl_t bno085_setting_tbl = {
    .PS0_Port = BNO085_PS0_GPIO_Port,
    .PS0_Pin = BNO085_PS0_Pin,
    .PS1_Port = BNO085_PS1_GPIO_Port,
    .PS1_Pin = BNO085_PS1_Pin,

    .RST_Port = BNO085_RST_GPIO_Port,
    .RST_Pin = BNO085_RST_Pin,

    .INT_Port = BNO085_INT_GPIO_Port,
    .INT_Pin = BNO085_INT_Pin,
    .bno_comm_type = BNO_COMM_TYPE,
    .init_state  = false,
};

bno085_tbl_t bno085_tbl = {
    .bno_seq = SEQ_HEADER,
    .INT_pinstate = false,
    .bno_data_size = 0,
};

#define BNOHEADER_SIZE 4
#define BNOBUFFER_SIZE 32768

uint8_t bnoHeader[BNOHEADER_SIZE];
uint8_t bnoBuffer[BNOBUFFER_SIZE];

static void bno085ChangeCommType(uint8_t type);


void bno085Reset(void);
void bno085WakeUp(void);
//GPIO_PinState bno085IntUpdate(void);
void bno085DataPrint(uint8_t* buffer, uint16_t len);
bool bno085GetInt(void);

void bno085Init(void)
{
  memset(bnoHeader,0,BNOHEADER_SIZE);
  memset(bnoBuffer,0,BNOBUFFER_SIZE);
  //통신 설정 초기화
  if(bno085_setting_tbl.PS0_Port != NULL && bno085_setting_tbl.PS1_Port != NULL)
  {
    bno085ChangeCommType(bno085_setting_tbl.bno_comm_type);
  }

  //리셋
  bno085Reset();
//  bno085WakeUp();
}

static void bno085ChangeCommType(uint8_t type)
{
  switch (type) {
    case BNO_COMM_I2C:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_RESET);
      break;
    case BNO_COMM_UART:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_RESET);
      break;
    case BNO_COMM_UART_RCV:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_SET);
      break;
    case BNO_COMM_SPI:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_SET);
      break;
    default:
      break;
  }
}

void bno085Main(void)
{
uint8_t dummy = 0x00;


  if(bno085_setting_tbl.init_state == false)
    return;

  switch (bno085_tbl.bno_seq) {
    case SEQ_HEADER:

//      if(bno085_tbl.INT_pinstate)
//        bno085_tbl.INT_pinstate = false;
//      else
//        break;
      if(bno085GetInt() == false)
        break;

      if(bno085SpiTransmitReceive(&dummy,bnoHeader,BNOHEADER_SIZE) != BNO_OK)
      {
        bno085_tbl.bno_seq = SEQ_ERROR;
        break;
      }
      bno085_tbl.bno_data_size = (uint16_t)bnoHeader[0] | ((uint16_t)bnoHeader[1] << 8);
      bno085_tbl.bno_data_size &= BNO_BUFFER_LIMIT;
      if(bno085_tbl.bno_data_size > 0)
        bno085_tbl.bno_seq = SEQ_DATA;
      break;

    case SEQ_DATA:

//      if(bno085_tbl.INT_pinstate)
//        bno085_tbl.INT_pinstate = false;
//      else
//        break;

      if(bno085GetInt() == false)
        break;

      if(bno085SpiTransmitReceive(&dummy,bnoBuffer,bno085_tbl.bno_data_size)!= BNO_OK)
      {
        bno085_tbl.bno_seq = SEQ_ERROR;
        break;
      }
      bno085_tbl.INT_pinstate = false;
      bno085_tbl.bno_seq = SEQ_HEADER;
//      bno085DataPrint(bnoBuffer, bno085_tbl.bno_data_size);
//      cliPrintf(bnoBuffer);
      break;
    case SEQ_ERROR:
      //리셋
      break;
    default:
      break;
  }

//  bno085IntUpdate();
//  if(bno085_tbl.INT_pinstate == true)
//  {
//    bno085_tbl.INT_pinstate = false;
//    bno085SpiReceive(bnoHeader,4);
//    bno085SpiReceive(bnoBuffer,4096);
//  }
}

BNO_STATE_E bno085SpiTransmit(uint8_t* pdata, uint32_t len)
{
  BNO_STATE_E ret = BNO_OK;
  if(spiTransmit(_DEF_BNO085_SPI, pdata, len) != HAL_OK)
  {
    ret = BNO_ERROR;
  }
  return ret;
}

BNO_STATE_E bno085SpiReceive(uint8_t* pdata, uint32_t len)
{
  BNO_STATE_E ret = BNO_OK;
  if(spiReceive(_DEF_BNO085_SPI, pdata, len)!= HAL_OK)
  {
    ret = BNO_ERROR;
  }
  return ret;
}

BNO_STATE_E bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t* r_pdata, uint32_t len)
{
  BNO_STATE_E ret = BNO_OK;
  if(bno085SpiTransmit(s_pdata, 1) != BNO_OK)
  {
    ret = BNO_ERROR;
    return ret;
  }
  if(bno085SpiReceive(r_pdata, len) != BNO_OK)
  {
    ret = BNO_ERROR;
    return ret;
  }
//  if(spiTransmitReceive(_DEF_BNO085_SPI, s_pdata, r_pdata, len) != HAL_OK)
//  {
//    ret = BNO_ERROR;
//  }
//  if(spiTransmitReceive(_DEF_BNO085_SPI, s_pdata, r_pdata, len) != HAL_OK)
//  {
//    ret = BNO_ERROR;
//  }
  return ret;
}


void bno085Reset(void)
{
  cliPrintf("BnoReset\n");
  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_SET);
  delay(100);
  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_RESET);
  delay(200);
  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_SET);
//  delay(100);
  bno085_setting_tbl.init_state = true;
//  if(bno085GetInt())
//  {
//    bno085_setting_tbl.init_state = true;
//  }
//  if(bno085GetInt() == true)
//  {
//    bno085_setting_tbl.init_state = true;
//  }
//  if(bno085_tbl.INT_pinstate == true)
//  {
//    cliPrintf("BnoResetOk!\n");
//    bno085_setting_tbl.init_state = true;
//    bno085_tbl.INT_pinstate = false;
//  }
//  else
//  {
//    cliPrintf("BnoResetFail!\n");
//    bno085_setting_tbl.init_state = false;
//  }

//  if(bno085WaitInt(500) == false)
//  {
//    cliPrintf("BnoResetFail!\n");
//    return;
//  }
//  cliPrintf("BnoResetOk!\n");

//  delay(50);
}

void bno085WakeUp(void)
{
  uint32_t timer = millis() + 500;
  bno085_tbl.INT_pinstate = false;
  HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_RESET);

  while((int32_t)(timer - millis()) > 0)
  {
    if(bno085_tbl.INT_pinstate == true)
    {
      cliPrintf("BnoWakeUP\n");
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
      return;
    }
    delay(20);
  }
  HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
  cliPrintf("BnoWakeUP Fail\n");
  //INT 핀 확인
}

uint32_t int_timer = 1000;
bool bno085GetInt(void)
{
  bool ret = false;
  GPIO_PinState pin_state = HAL_GPIO_ReadPin(bno085_setting_tbl.INT_Port, bno085_setting_tbl.INT_Pin);

  if(pin_state == GPIO_PIN_RESET)
  {
    bno085_tbl.INT_pinstate = true;
//    int_timer = millis() + 1000;
    ret = true;
  }
  else
  {
    bno085_tbl.INT_pinstate = false;
  }

//  if(int_timer < millis())
//  {
////    bno085Reset();
//  }

//  bno085Reset();
  return ret;
}

void bno085DataPrint(uint8_t* buffer, uint16_t len)
{
  for(int i = 0; i<len; i++)
  {
    if(i % 10 == 0)
      cliPrintf("\n");
    cliPrintf("%02X ",buffer[i]);
  }
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//  if(GPIO_Pin == GPIO_PIN_2)
//  {
//    bno085_tbl.INT_pinstate = true;
//  }
//}

//GPIO_PinState bno085IntUpdate(void)
//{
//  GPIO_PinState ret = HAL_GPIO_ReadPin(bno085_setting_tbl.INT_Port, bno085_setting_tbl.INT_Pin);
//  if(ret == GPIO_PIN_RESET)bno085_tbl.INT_pinstate = true;
//  return ret;
//}
