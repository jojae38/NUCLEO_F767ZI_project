/*
 * bno085.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "bno085.h"

void bno085Init(void)
{
#ifdef BNO_SPI
  bnoSpiReset();
#endif
}


void bno085Main(void)
{
#ifdef BNO_SPI
  bnoSpiSeq();
#endif
}

//CSN은 하지 않는 상태에서 SCK만 동기화


//static void bno085ChangeCommType(uint8_t type)
//{
//#ifdef BNO_I2C
//  PS0_wake(false);
//  PS1(false);
//#endif
//
//#ifdef BNO_UART
//  PS0_wake(true);
//  PS1(false);
//#endif
//
//#ifdef BNO_UARTRVC
//  PS0_wake(false);
//  PS1(true);
//#endif
//
//#ifdef BNO_SPI
//  PS0_wake(true);
//  PS1(true);
//#endif
//}
//
//void bno085SpiRead(uint8_t*buffer, uint16_t len)
//{
//  if(!bno085WaitInt(500))
//    return;
//
//  uint8_t dummy = 0x00;
//  uint8_t header_buffer[4];
//  memset(header_buffer,0,4);
//
//  if (bno085SpiTransmitReceive(&dummy, 1, header_buffer, 4) == false) {
//    return;
//  }
//
////  if(bno085SpiTransmitReceiveOrg(&dummy, header_buffer, 4) != BNO_OK)
////  {
////    return;
////  }
//
//  uint16_t packet_size = 0;
//  packet_size = (uint16_t)header_buffer[0] | (uint16_t)header_buffer[1] << 8;
//  packet_size &= ~0x8000;
//
//  if (packet_size > len) {
//    return;
//  }
//
//  if(!bno085WaitInt(500))
//    return;
//
//  if (bno085SpiTransmitReceive(&dummy, 1, buffer, packet_size) == false) {
////    cliPrintf("ReadFail\r\n");
//    return;
//  }
//  cliPrintf("PacketSize : %i\r\n",packet_size);
////  if(bno085SpiTransmitReceiveOrg(&dummy, buffer, packet_size) != BNO_OK)
////  {
////    cliPrintf("ReadFail\r\n");
////    return;
////  }
//  bno085DataPrint(buffer,packet_size);
//  return;
//}


