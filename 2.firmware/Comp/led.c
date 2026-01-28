/*
 * led.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "led.h"

led_tbl_t led_tbl[HW_LED_MAX_CH] =
{
    {.port = LD1_GPIO_Port, .pin = LD1_Pin, .on_state = GPIO_PIN_SET, .off_state = GPIO_PIN_RESET, .state = false},
    {.port = LD2_GPIO_Port, .pin = LD2_Pin, .on_state = GPIO_PIN_SET, .off_state = GPIO_PIN_RESET, .state = false},
    {.port = LD3_GPIO_Port, .pin = LD3_Pin, .on_state = GPIO_PIN_SET, .off_state = GPIO_PIN_RESET, .state = false}
};

void ledInit(void)
{
  for(int i = 0; i < HW_LED_MAX_CH; i++)
  {
    ledOff(i);
  }
}

void ledOn(uint8_t ch)
{
  led_tbl[ch].state = true;
  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  led_tbl[ch].state = false;
  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if(led_tbl[ch].state == false)
    ledOn(ch);
  else
    ledOff(ch);
}
