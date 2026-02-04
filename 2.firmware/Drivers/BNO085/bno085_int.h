/*
 * bno085_int.h
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */

#ifndef BNO085_BNO085_INT_H_
#define BNO085_BNO085_INT_H_

#include "bno085_common.h"

#ifdef BNO_SPI
bool extiIntCheck(bool reset);
bool waitnterruptCheck(bool reset);
#endif

#endif /* BNO085_BNO085_INT_H_ */
