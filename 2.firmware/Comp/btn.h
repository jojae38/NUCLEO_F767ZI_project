/*
 * btn.h
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */

#ifndef BTN_H_
#define BTN_H_

#include "hw_def.h"

typedef struct{
  GPIO_TypeDef* port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
  uint32_t      duration;
}btn_tbl_t;

//100ms 타이머에 넣을 시
#define BTN_DURATION_STEP   100

void btnInit(void);
void btnMain(void);
uint32_t getBtnState(uint8_t ch);
void resetBtnState(uint8_t ch);

#endif /* BTN_H_ */
