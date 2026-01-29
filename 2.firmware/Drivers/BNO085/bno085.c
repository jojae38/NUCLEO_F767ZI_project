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

static void bno085ChangeCommType(uint8_t type);

static inline bno085SpiTransmit(void);
static inline bno085SpiReceive(void);
static inline bno085SpiTransmitReceive(void);

void bno085Init(void)
{
  //통신 설정 초기화
  if(bno085_setting_tbl.PS0_Port != NULL && bno085_setting_tbl.PS1_Port != NULL)
  {
    bno085ChangeCommType(bno085_setting_tbl.bno_comm_type);
  }
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

}

void bno085WakeUp(void)
{
  //INT 핀 확인
}
