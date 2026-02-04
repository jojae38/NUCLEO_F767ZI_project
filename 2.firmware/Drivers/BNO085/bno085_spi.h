/*
 * bno085_spi.h
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#ifndef BNO085_BNO085_SPI_H_
#define BNO085_BNO085_SPI_H_

#include "bno085_common.h"

typedef enum{
  SPI_INIT = 0,
  SPI_DUMMY,
  SPI_IDLE,
  SPI_RD_HDR,
  SPI_RD_BODY,
  SPI_WRITE,
  SEQ_ERROR = 99,
}BNO_SPI_SEQ;

void bnoSpiReset(void);
void bnoSpiSeq(void);

#endif /* BNO085_BNO085_SPI_H_ */
