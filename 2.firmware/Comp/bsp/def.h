/*
 * def.h
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */

#ifndef BSP_DEF_H_
#define BSP_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef uint8_t   BYTE;             // 8-bit byte
typedef uint32_t  WORD;             // 32-bit word, change to "long" for 16-bit machines

#define __vo volatile
// uint16_t 버전
static inline void Set_Flag16(uint16_t *flags, uint16_t mask) {
    *flags |= mask;
}
static inline void Clear_Flag16(uint16_t *flags, uint16_t mask) {
    *flags &= ~mask;
}
static inline bool Is_Flag_Set16(uint16_t flags, uint16_t mask) {
    return (flags & mask) != 0;
}

#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))

#define SECOND(sec)   ((sec) * 1000)
#define MINUTE(min)   ((min) * SECOND(60))

//UART
#define _DEF_UART2_CLI        0
#define _DEF_UART4_BNO        1

//SPI
#define _DEF_BNO085_SPI	      0

//BNO085 ? BNO080
//#define _USE_BNO080
//#define _USE_BNO085

#endif /* BSP_DEF_H_ */
