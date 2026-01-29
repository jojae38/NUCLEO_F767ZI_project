/*
 * btn.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "btn.h"

btn_tbl_t btn_tbl[HW_BTN_MAX_CH] = {
    {.port = USER_Btn_GPIO_Port, .pin = USER_Btn_Pin,.on_state = GPIO_PIN_SET,.off_state = GPIO_PIN_RESET,.duration = 0},
};

void btnInit(void)
{
  return;
}

void btnMain(void)
{
  for(int i = 0; i< HW_BTN_MAX_CH; i++)
  {
    GPIO_PinState tmp_state = HAL_GPIO_ReadPin(btn_tbl[i].port, btn_tbl[i].pin);
    if(btn_tbl[i].on_state == tmp_state)
    {
      btn_tbl[i].duration += BTN_DURATION_STEP;

    }
    else
    {
      btn_tbl[i].duration = 0;
    }
  }
}

uint32_t getBtnState(uint8_t ch)
{
  uint32_t ret = btn_tbl[ch].duration;
  return ret;
}

void resetBtnState(uint8_t ch)
{
  btn_tbl[ch].duration = 0;
}
