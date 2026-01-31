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

//    .INT_Port = BNO085_INT_GPIO_Port,
//    .INT_Pin = BNO085_INT_Pin,
    .bno_comm_type = BNO_COMM_TYPE,
    .init_state  = false,
};

bno085_tbl_t bno085_tbl = {
    .INT_pinstate = false,

};

uint8_t bnoHeader[4];
uint8_t bnoBuffer[4096];

static void bno085ChangeCommType(uint8_t type);


void bno085Reset(void);
void bno085WakeUp(void);
//GPIO_PinState bno085IntUpdate(void);
bool bno085WaitInt(uint32_t term);
void bno085Init(void)
{
  memset(bnoHeader,0,4);
  memset(bnoBuffer,0,4096);
  //통신 설정 초기화
  if(bno085_setting_tbl.PS0_Port != NULL && bno085_setting_tbl.PS1_Port != NULL)
  {
    bno085ChangeCommType(bno085_setting_tbl.bno_comm_type);
  }

//  HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
//  HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_SET);
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
  if(spiTransmitReceive(_DEF_BNO085_SPI,s_pdata, r_pdata, len) != HAL_OK)
  {
    ret = BNO_ERROR;
  }
  return ret;
}


void bno085Reset(void)
{

  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_SET);
  delay(100);
  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_RESET);
  delay(200);
  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_SET);
  delay(100);

  if(bno085_tbl.INT_pinstate == true)
  {
    cliPrintf("BnoResetOk!\n");
    bno085_setting_tbl.init_state = true;
  }
  else
  {
    cliPrintf("BnoResetFail!\n");
    bno085_setting_tbl.init_state = false;
  }

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

bool bno085WaitInt(uint32_t term)
{
  uint32_t timer = millis() + 500;
  while(timer > millis())
  {
    if(bno085_tbl.INT_pinstate == true)
      return true;
  }

  return false;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_2)
  {
    bno085_tbl.INT_pinstate = true;
  }
}


//GPIO_PinState bno085IntUpdate(void)
//{
//  GPIO_PinState ret = HAL_GPIO_ReadPin(bno085_setting_tbl.INT_Port, bno085_setting_tbl.INT_Pin);
//  if(ret == GPIO_PIN_RESET)bno085_tbl.INT_pinstate = true;
//  return ret;
//}
