/*
 * bno085_common.h
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#ifndef BNO085_BNO085_COMMON_H_
#define BNO085_BNO085_COMMON_H_

#include "hw_def.h"
#include "cli.h"

//COMM TYPE
#define BNO_SPI
//#define BNO_I2C
//#define BNO_UART
//#define BNO_UARTRVC

//PIN ANNOUNCE
#define BNO_RST_PORT    BNO085_RST_GPIO_Port
#define BNO_RST_PIN     BNO085_RST_Pin
#define BNO_PS0_PORT    BNO085_PS0_GPIO_Port
#define BNO_PS0_PIN     BNO085_PS0_Pin
#define BNO_PS1_PORT    BNO085_PS1_GPIO_Port
#define BNO_PS1_PIN     BNO085_PS1_Pin
#define BNO_INT_PORT    BNO085_INT_GPIO_Port
#define BNO_INT_PIN     BNO085_INT_Pin

#ifdef BNO_SPI
extern SPI_HandleTypeDef hspi3;
#define BNO_SPI_HANDLER   hspi3
#endif

#ifdef BNO_I2C
#define BNO_ADDR7  0x4A
#define BNO_ADDR8  (BNO_ADDR7 << 1)
#define I2C_BUFFER_MAX 1024
#endif

#ifdef BNO_UART

#endif

#ifdef BNO_UARTRVC

#endif

//COMMON DEFINE
#define BNO_BUFFER_LIMIT        0x7FFF
#define BNO_DEFAULT_WAIT_TIME   500
#define BNO_TX_BUFFER_LIMIT     512
#define BNO_RX_BUFFER_LIMIT     512

typedef struct{
  uint32_t raw_acc;

}bno085_sensor_t;

typedef enum{
  BNO_OK = 0,
  BNO_ERROR,
  BNO_TIMEOUT,
}BNO_STATE_E;

void PS0_wake(bool state);
void PS1(bool state);
void RSTN(bool state);
bool getIntState(void);

#endif /* BNO085_BNO085_COMMON_H_ */
