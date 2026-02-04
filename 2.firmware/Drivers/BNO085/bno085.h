/*
 * bno085.h
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */

/*
 *인터럽트 핀은 Falling Edge Detect
 *
 *
 *UART - RVC 쓰면 100hz단위로 데이터가 오는듯
 *그 외 I2C, SPI 쓰면 SHTP라는 프로토콜을 사용함
 *
 *
 * */

#ifndef BNO085_BNO085_H_
#define BNO085_BNO085_H_

#include "bno085_common.h"
#include "bno085_spi.h"
#include "bno085_i2c.h"
#include "bno085_uart.h"
#include "bno085_uartrvc.h"

typedef struct{
  bool inReset;

}bno085_state_t;

typedef struct{
  bool INT_pinstate;
  uint16_t bno_data_size;
  uint32_t rx_time_stamp;
  uint32_t rx_time_interval;
}bno085_tbl_t;

void bno085Init(void);
void bno085Main(void);

#endif /* BNO085_BNO085_H_ */
