/*
 * bno085_common.c
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#include "bno085_common.h"
#include "cli.h"

void PS0_wake(bool state){HAL_GPIO_WritePin(BNO_PS0_PORT, BNO_PS0_PIN, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
void PS1(bool state){HAL_GPIO_WritePin(BNO_PS1_PORT, BNO_PS1_PIN, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
void RSTN(bool state){HAL_GPIO_WritePin(BNO_RST_PORT, BNO_RST_PIN, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
bool getIntState(void){
  if(HAL_GPIO_ReadPin(BNO085_INT_GPIO_Port, BNO085_INT_Pin) == GPIO_PIN_SET) return true;
  else return false;
}
