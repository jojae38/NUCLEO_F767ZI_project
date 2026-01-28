/*
 * led.h
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */

#ifndef LED_H_
#define LED_H_

#include "hw_def.h"

typedef struct{
  GPIO_TypeDef* port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
  bool          state;
}led_tbl_t;

void ledInit(void);
void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);

#endif /* LED_H_ */
