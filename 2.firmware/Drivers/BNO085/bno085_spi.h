/*
 * bno085_spi.h
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#ifndef BNO085_BNO085_SPI_H_
#define BNO085_BNO085_SPI_H_

#include "bno085_common.h"
#include "sh2.h"

typedef enum{
  SPI_INIT = 0,
  SPI_DUMMY,
  SPI_IDLE,
  SPI_RD_HDR_WAIT,
  SPI_RD_HDR,
  SPI_RD_BODY_WAIT,
  SPI_RD_BODY,
  SPI_WRITE_WAIT,
  SPI_WRITE,
  SEQ_ERROR = 99,
}BNO_SPI_SEQ;

void bnoSpiReset(void);
void bnoSpiSeq(void);

sh2_Hal_t* sh2Spi_init(void);
int sh2SpiOpen(sh2_Hal_t *self);
void sh2SpiClose(sh2_Hal_t *self);
int sh2SpiRead(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len, uint32_t *t_us);
int sh2SpiWrite(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len);
uint32_t sh2SpiMicros(sh2_Hal_t *self);

#endif /* BNO085_BNO085_SPI_H_ */
