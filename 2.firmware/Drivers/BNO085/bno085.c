/*
 * bno085.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "bno085.h"
#include "_spi.h"

bno085_setting_tbl_t bno085_setting_tbl = {
    .PS0_Port = BNO085_PS0_GPIO_Port,
    .PS0_Pin = BNO085_PS0_Pin,
    .PS1_Port = BNO085_PS1_GPIO_Port,
    .PS1_Pin = BNO085_PS1_Pin,
    .bno_comm_type = BNO_COMM_TYPE,
};

bno085_tbl_t bno085_tbl = {
    .INT_pinstate = false,

};

uint8_t bnoHeader[4];
uint8_t bnoBuffer[4096];

static void bno085ChangeCommType(uint8_t type);

static BNO_STATE_E bno085SpiTransmit(uint8_t* pdata, uint32_t len);
static BNO_STATE_E bno085SpiReceive(uint8_t* pdata, uint32_t len);
static BNO_STATE_E bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t* r_pdata, uint32_t len);
void bno085Reset(void);

void bno085Init(void)
{
  memset(bnoHeader,0,4);
  memset(bnoBuffer,0,4096);
  //통신 설정 초기화
  if(bno085_setting_tbl.PS0_Port != NULL && bno085_setting_tbl.PS1_Port != NULL)
  {
    bno085ChangeCommType(bno085_setting_tbl.bno_comm_type);
  }

  //리셋
  bno085Reset();
}

static void bno085ChangeCommType(uint8_t type)
{
  switch (type) {
    case 0:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_RESET);
      break;
    case 1:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_RESET);
      break;
    case 2:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_SET);
      break;
    case 3:
      HAL_GPIO_WritePin(bno085_setting_tbl.PS0_Port, bno085_setting_tbl.PS0_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(bno085_setting_tbl.PS1_Port, bno085_setting_tbl.PS1_Pin, GPIO_PIN_SET);
      break;
    default:
      break;
  }
}

void bno085Main(void)
{
  if(bno085_tbl.INT_pinstate == true)
  {
    bno085_tbl.INT_pinstate = false;
    bno085SpiReceive(bnoHeader,4);
    bno085SpiReceive(bnoBuffer,4096);
  }
}

static BNO_STATE_E bno085SpiTransmit(uint8_t* pdata, uint32_t len)
{
  BNO_STATE_E ret = BNO_OK;
  if(spiTransmit(_DEF_BNO085_SPI, pdata, len) != HAL_OK)
  {
    ret = BNO_ERROR;
  }
  return ret;
}

static BNO_STATE_E bno085SpiReceive(uint8_t* pdata, uint32_t len)
{
  BNO_STATE_E ret = BNO_OK;
  if(spiReceive(_DEF_BNO085_SPI, pdata, len)!= HAL_OK)
  {
    ret = BNO_ERROR;
  }
  return ret;
}

static BNO_STATE_E bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t* r_pdata, uint32_t len)
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
  delay(20);
  HAL_GPIO_WritePin(bno085_setting_tbl.RST_Port, bno085_setting_tbl.RST_Pin, GPIO_PIN_RESET);
  delay(200);
}

void bno085WakeUp(void)
{
  //INT 핀 확인
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_2)
  {
    bno085_tbl.INT_pinstate = true;

  }
}
