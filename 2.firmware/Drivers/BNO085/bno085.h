/*
 * bno085.h
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */

#ifndef BNO085_BNO085_H_
#define BNO085_BNO085_H_

#include "hw_def.h"

// I2C - 0, UART - 1, UART-RCV - 2, SPI - 3
typedef enum {
    BNO_COMM_I2C = 0,
    BNO_COMM_UART,
    BNO_COMM_UART_RCV,
    BNO_COMM_SPI,
} BNO_COMM_TYPE_E;

typedef enum{
  BNO_OK = 0,
  BNO_ERROR = 1,
  BNO_TIMEOUT = 2,
}BNO_STATE_E;

#define BNO_COMM_TYPE   BNO_COMM_SPI
#define BNO_I2C_ADDR    0x4A

typedef struct{
//PS0 PS1 핀 세팅
  GPIO_TypeDef*   PS0_Port;
  uint16_t        PS0_Pin;
  GPIO_TypeDef*   PS1_Port;
  uint16_t        PS1_Pin;
  BNO_COMM_TYPE_E bno_comm_type;
//리셋 핀
  GPIO_TypeDef*   RST_Port;
  uint16_t        RST_Pin;
//인터럽트 핀
  GPIO_TypeDef*   INT_Port;
  uint16_t        INT_Pin;
}bno085_setting_tbl_t;

typedef struct{
  bool INT_pinstate;

}bno085_tbl_t;

void bno085Init(void);
void bno085Main(void);

#endif /* BNO085_BNO085_H_ */
