/*
 * hw.h
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */

#ifndef HW_HW_H_
#define HW_HW_H_

#include "hw_def.h"
#include "cli.h"
#include "led.h"
#include "btn.h"
#include "BNO080.h"
#include "BNO085.h"
//#include "Quaternion.h"

void hwInit(void);
void hwMain(void);

#endif /* HW_HW_H_ */
