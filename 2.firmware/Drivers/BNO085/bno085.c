/*
 * bno085.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: Pro
 */


#include "bno085.h"
#include "_spi.h"
#include "cli.h"
#include "_uart.h"
#include "led.h"

extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi3;

//static void NSS_L(){ HAL_GPIO_WritePin(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, GPIO_PIN_RESET); }
//static void NSS_H(){ HAL_GPIO_WritePin(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, GPIO_PIN_SET); }
//ACTIVE LOW

static void PS0_wake(bool state){HAL_GPIO_WritePin(BNO085_PS0_GPIO_Port, BNO085_PS0_Pin, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
static void PS1(bool state){HAL_GPIO_WritePin(BNO085_PS1_GPIO_Port, BNO085_PS1_Pin, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
static void NSS(bool state){HAL_GPIO_WritePin(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, state? GPIO_PIN_SET : GPIO_PIN_RESET);}
static void RSTN(bool state){HAL_GPIO_WritePin(BNO085_RST_GPIO_Port, BNO085_RST_Pin, state? GPIO_PIN_SET : GPIO_PIN_RESET);}

bno085_setting_tbl_t bno085_setting_tbl = {
    .i2c_handler = &hi2c2,
    .spi_handler = &hspi3,
    .bno_comm_type = BNO_COMM_TYPE,
    .init_state  = false,
};

bno085_tbl_t bno085_tbl = {
    .bno_seq = SPI_INIT,
    .INT_pinstate = false,
    .bno_data_size = 0,
};

uint8_t tmp_seq;
uint32_t int_count;

#define BNOHEADER_SIZE 4
#define BNOBUFFER_SIZE 32768

uint8_t bnoHeader[BNOHEADER_SIZE];
uint8_t bnoBuffer[BNOBUFFER_SIZE];

static void bno085ChangeCommType(uint8_t type);
BNO_STATE_E bno085SpiTransmitReceiveOrg(uint8_t* s_pdata, uint8_t* r_pdata, uint32_t len);

void bno085Reset(void);
bool bno085WakeUp(void);
//GPIO_PinState bno085IntUpdate(void);
void bno085DataPrint(uint8_t* buffer, uint16_t len);
bool bno085GetInt(void);
bool bno085WaitInt(uint32_t term);

bool bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t s_len, uint8_t* r_pdata, uint32_t r_len);

void bno085Seq(void);
bool bno085I2CReceive(uint8_t* pdata, uint16_t len);

void bno085Init(void)
{
  int_count = 0;
  tmp_seq = 0;
  memset(bnoHeader,0,BNOHEADER_SIZE);
  memset(bnoBuffer,0,BNOBUFFER_SIZE);
  uartOpen(_DEF_UART4_BNO,115200,bnoBuffer,BNOBUFFER_SIZE);
  //통신 설정 초기화
  bno085ChangeCommType(bno085_setting_tbl.bno_comm_type);
  //리셋
  delay(10);
  bno085Reset();
//  if(bno085WakeUp())
//    cliPrintf("BnoWake");
//
//  delay(10);
//  if(bno085WakeUp())
//    cliPrintf("BnoWake");
}

//CSN은 하지 않는 상태에서 SCK만 동기화
void bno085DummyOp(void)
{
  uint8_t dummyTx[1];
  uint8_t dummyRx[1];
  memset(dummyTx,0xAA,sizeof(dummyTx));
  HAL_SPI_TransmitReceive(bno085_setting_tbl.spi_handler, dummyTx, dummyRx, sizeof(dummyTx), 10);
}

static void bno085ChangeCommType(uint8_t type)
{
  switch (type) {
    case BNO_COMM_I2C:
      PS0_wake(false);
      PS1(false);
      break;
    case BNO_COMM_UART:
      PS0_wake(true);
      PS1(false);
      break;
    case BNO_COMM_UART_RCV:
      PS0_wake(false);
      PS1(true);
      break;
    case BNO_COMM_SPI:
      PS0_wake(true);
      PS1(true);
      break;
    default:
      break;
  }
}

void bno085SpiRead(uint8_t*buffer, uint16_t len)
{
  if(!bno085WaitInt(500))
    return;

  uint8_t dummy = 0x00;
  uint8_t header_buffer[4];
  memset(header_buffer,0,4);

  if (bno085SpiTransmitReceive(&dummy, 1, header_buffer, 4) == false) {
    return;
  }

//  if(bno085SpiTransmitReceiveOrg(&dummy, header_buffer, 4) != BNO_OK)
//  {
//    return;
//  }

  uint16_t packet_size = 0;
  packet_size = (uint16_t)header_buffer[0] | (uint16_t)header_buffer[1] << 8;
  packet_size &= ~0x8000;

  if (packet_size > len) {
    return;
  }

  if(!bno085WaitInt(500))
    return;

  if (bno085SpiTransmitReceive(&dummy, 1, buffer, packet_size) == false) {
//    cliPrintf("ReadFail\r\n");
    return;
  }
  cliPrintf("PacketSize : %i\r\n",packet_size);
//  if(bno085SpiTransmitReceiveOrg(&dummy, buffer, packet_size) != BNO_OK)
//  {
//    cliPrintf("ReadFail\r\n");
//    return;
//  }
  bno085DataPrint(buffer,packet_size);
  return;
}

//static int spihal_write(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len) {
//  // Serial.print("SPI HAL write packet size: ");
//  // Serial.println(len);
//
//  if (!spihal_wait_for_int()) {
//    return 0;
//  }
//
//  spi_dev->write(pBuffer, len);
//
//  return len;
//
//}
//void bno085Seq(void)
//{
//  uint8_t dummy = 0x00;
//    if(bno085_setting_tbl.init_state == false)
//      return;
//
//    switch (bno085_tbl.bno_seq) {
//      case SEQ_HEADER:
//
//  //      if(bno085_tbl.INT_pinstate)
//  //        bno085_tbl.INT_pinstate = false;
//  //      else
//  //        break;
//        if(bno085GetInt() == false)
//          break;
//
//        if(bno085SpiTransmitReceive(&dummy,bnoHeader,BNOHEADER_SIZE) != BNO_OK)
//        {
//          bno085_tbl.bno_seq = SEQ_ERROR;
//          break;
//        }
//        bno085_tbl.bno_data_size = (uint16_t)bnoHeader[0] | ((uint16_t)bnoHeader[1] << 8);
//        bno085_tbl.bno_data_size &= BNO_BUFFER_LIMIT;
//        if(bno085_tbl.bno_data_size > 0)
//          bno085_tbl.bno_seq = SEQ_DATA;
//        break;
//
//      case SEQ_DATA:
//
//  //      if(bno085_tbl.INT_pinstate)
//  //        bno085_tbl.INT_pinstate = false;
//  //      else
//  //        break;
//
//        if(bno085GetInt() == false)
//          break;
//
//        if(bno085SpiTransmitReceive(&dummy,bnoBuffer,bno085_tbl.bno_data_size)!= BNO_OK)
//        {
//          bno085_tbl.bno_seq = SEQ_ERROR;
//          break;
//        }
//        bno085_tbl.INT_pinstate = false;
//        bno085_tbl.bno_seq = SEQ_HEADER;
//  //      bno085DataPrint(bnoBuffer, bno085_tbl.bno_data_size);
//  //      cliPrintf(bnoBuffer);
//        break;
//      case SEQ_ERROR:
//        //리셋
//        break;
//      default:
//        break;
//    }
//}

void bno085Main(void)
{
  bno085SpiRead(bnoBuffer,BNOBUFFER_SIZE);
//  bno085Seq();
}


//void bno085Seq(void)
//{
//  bno085I2CReceive(bnoBuffer,BNOBUFFER_SIZE);
//}

#define BNO_ADDR7  0x4A
#define BNO_ADDR8  (BNO_ADDR7 << 1)
#define I2C_BUFFER_MAX 1024
bool bno085I2CTransmit(uint8_t* pdata, uint32_t len)
{
  if (HAL_I2C_Master_Transmit(bno085_setting_tbl.i2c_handler, BNO_ADDR8, pdata, len, 500) != HAL_OK)
    return false;
  return true;
}

bool bno085I2CReceive(uint8_t* pdata, uint16_t len)
{
  uint8_t header[4];
  uint8_t read_addr = (BNO_ADDR8 + 1);
  // 1) header
  if (HAL_I2C_Master_Receive(bno085_setting_tbl.i2c_handler, read_addr, header, 4, 500) != HAL_OK)
    return false;

  uint16_t packet_size = (uint16_t)header[0] | ((uint16_t)header[1] << 8);
  packet_size &= 0x7FFF; // continuation bit 제거


  uint16_t cargo_remaining = packet_size;
  uint8_t i2c_buffer[I2C_BUFFER_MAX];
  uint16_t read_size;
  uint16_t cargo_read_amount = 0;
  bool first_read = true;

  while (cargo_remaining > 0) {
    if (first_read) {
      read_size = MIN(I2C_BUFFER_MAX, (size_t)cargo_remaining);
    } else {
      read_size = MIN(I2C_BUFFER_MAX, (size_t)cargo_remaining + 4);
    }

        // Serial.print("Reading from I2C: "); Serial.println(read_size);
        // Serial.print("Remaining to read: "); Serial.println(cargo_remaining);

    if (HAL_I2C_Master_Receive(bno085_setting_tbl.i2c_handler, BNO_ADDR8, i2c_buffer, read_size, 500) != HAL_OK) {
      return 0;
    }

    if (first_read) {
          // The first time we're saving the "original" header, so include it in the
          // cargo count
      cargo_read_amount = read_size;
      memcpy(pdata, i2c_buffer, cargo_read_amount);
      first_read = false;
    } else {
          // this is not the first read, so copy from 4 bytes after the beginning of
          // the i2c buffer to skip the header included with every new i2c read and
          // don't include the header in the amount of cargo read
      cargo_read_amount = read_size - 4;
      memcpy(pdata, i2c_buffer + 4, cargo_read_amount);
    }
        // advance our pointer by the amount of cargo read
    pdata += cargo_read_amount;
        // mark the cargo as received
    cargo_remaining -= cargo_read_amount;
  }

  return true;
}


//0 Write
//bool bno085I2CTransmit(uint8_t* pdata, uint32_t len)
//{
//  bool ret = false;
//  uint8_t write_address = ((uint8_t)BNO_I2C_ADDR & (~0x80));
//  HAL_I2C_Master_Transmit(bno085_setting_tbl.i2c_handler, write_address, pdata, len, 500);
//
//
//  return ret;
//}
//
//#define I2C_BUFFER_MAX 1024
//bool bno085I2CReceive(uint8_t* pdata, uint32_t len)
//{
//  bool ret = false;
//
//  uint8_t header[4];
//  uint8_t read_address = ((uint8_t)BNO_I2C_ADDR | 0x80);
//  if(HAL_I2C_Master_Receive(bno085_setting_tbl.i2c_handler, read_address, header, 4, 500) != HAL_OK)
//    return false;
//
//  uint16_t packet_size = ((uint16_t)header[0]|(uint16_t)header[1]<<8);
//  packet_size &= BNO_BUFFER_LIMIT;
//
//  uint16_t cargo_remaining = packet_size;
//  uint8_t i2c_buffer[I2C_BUFFER_MAX];
//  uint16_t read_size;
//  uint16_t cargo_read_amount = 0;
//  bool first_read = true;
//
//  while (cargo_remaining > 0) {
//    if (first_read) {
//      read_size = MIN(I2C_BUFFER_MAX, (size_t)cargo_remaining);
//    } else {
//      read_size = MIN(I2C_BUFFER_MAX, (size_t)cargo_remaining + 4);
//    }
//
//      // Serial.print("Reading from I2C: "); Serial.println(read_size);
//      // Serial.print("Remaining to read: "); Serial.println(cargo_remaining);
//
//    if (HAL_I2C_Master_Receive(bno085_setting_tbl.i2c_handler, read_address, i2c_buffer, read_size, 500) != HAL_OK) {
//      return 0;
//    }
//
//    if (first_read) {
//        // The first time we're saving the "original" header, so include it in the
//        // cargo count
//      cargo_read_amount = read_size;
//      memcpy(pdata, i2c_buffer, cargo_read_amount);
//      first_read = false;
//    } else {
//        // this is not the first read, so copy from 4 bytes after the beginning of
//        // the i2c buffer to skip the header included with every new i2c read and
//        // don't include the header in the amount of cargo read
//      cargo_read_amount = read_size - 4;
//      memcpy(pdata, i2c_buffer + 4, cargo_read_amount);
//    }
//      // advance our pointer by the amount of cargo read
//    pdata += cargo_read_amount;
//      // mark the cargo as received
//    cargo_remaining -= cargo_read_amount;
//  }
//  return ret;
//}

//BNO_STATE_E bno085SpiTransmit(uint8_t* pdata, uint32_t len)
//{
//  BNO_STATE_E ret = BNO_OK;
//  if(spiTransmit(_DEF_BNO085_SPI, pdata, len) != HAL_OK)
//  {
//    ret = BNO_ERROR;
//  }
//  return ret;
//}
//
//BNO_STATE_E bno085SpiReceive(uint8_t* pdata, uint32_t len)
//{
//  BNO_STATE_E ret = BNO_OK;
//  if(spiReceive(_DEF_BNO085_SPI, pdata, len)!= HAL_OK)
//  {
//    ret = BNO_ERROR;
//  }
//  return ret;
//}
//
//BNO_STATE_E bno085SpiTransmitReceiveOrg(uint8_t* s_pdata, uint8_t* r_pdata, uint32_t len)
//{
//  BNO_STATE_E ret = BNO_OK;
//  if(bno085SpiTransmit(s_pdata, 1) != BNO_OK)
//  {
//    ret = BNO_ERROR;
//    return ret;
//  }
//  if(bno085SpiReceive(r_pdata, len) != BNO_OK)
//  {
//    ret = BNO_ERROR;
//    return ret;
//  }
////  if(spiTransmitReceive(_DEF_BNO085_SPI, s_pdata, r_pdata, len) != HAL_OK)
////  {
////    ret = BNO_ERROR;
////  }
////  if(spiTransmitReceive(_DEF_BNO085_SPI, s_pdata, r_pdata, len) != HAL_OK)
////  {
////    ret = BNO_ERROR;
////  }
//  return ret;
//}


//bool bno085SpiTransmit(uint8_t* pdata, uint32_t len)
//{
//  NSS_L();
//  if(HAL_SPI_Transmit(bno085_setting_tbl.spi_handler, s_pdata, s_len, 500) != HAL_OK)
//    return false;
//  NSS_H();
//  return true;
//}
//
//bool bno085SpiReceive(uint8_t* pdata, uint32_t len)
//{
//  NSS_L();
//  if(HAL_SPI_Receive(bno085_setting_tbl.spi_handler, s_pdata, s_len, 500) != HAL_OK)
//    return false;
//  NSS_H();
//  return true;
//}

bool bno085SpiTransmitReceive(uint8_t* s_pdata, uint8_t s_len, uint8_t* r_pdata, uint32_t r_len)
{
  NSS(false);
  if(HAL_SPI_Transmit(bno085_setting_tbl.spi_handler, s_pdata, s_len, 500) != HAL_OK)
    goto error;
  if(HAL_SPI_Receive(bno085_setting_tbl.spi_handler, r_pdata, r_len, 500) != HAL_OK)
    goto error;

  NSS(true);
  return true;

error:
  NSS(true);
  return false;

}

void bno085Reset(void)
{
  RSTN(true);
  delay(100);
  RSTN(false);
  delay(10);
  RSTN(true);
  if(bno085WaitInt(500))
  {
    bno085_setting_tbl.init_state = true;
    cliPrintf("BnoResetOk\r\n");
  }
  else
  {
    bno085_setting_tbl.init_state = false;
    cliPrintf("BnoResetFail\r\n");
  }
//  delay(10);

//  delay(150);
////  bno085_setting_tbl.init_state = true;
//  if(bno085GetInt())
//  {
//    bno085_setting_tbl.init_state = true;
//    cliPrintf("BnoResetOk\r\n");
//  }
//  else
//  {
//    bno085_setting_tbl.init_state = false;
//    cliPrintf("BnoResetFail\r\n");
//  }
}

bool bno085WakeUp(void)
{
  PS0_wake(false);
  if(!bno085WaitInt(500))
  {
    return false;
  }
  PS0_wake(true);
  delay(1);
  return true;
}

uint32_t int_timer = 1000;
bool bno085GetInt(void)
{
  bool ret = bno085_tbl.INT_pinstate;
  bno085_tbl.INT_pinstate = false;
  return ret;
}

bool bno085WaitInt(uint32_t term)
{
  uint32_t timer = millis() + term;
  while(timer > millis())
  {
    if(bno085GetInt())
      return true;

    delay(1);
  }
  return false;
}

void bno085DataPrint(uint8_t* buffer, uint16_t len)
{
  for(int i = 0; i<len; i++)
  {
    if(i % 10 == 0)
      cliPrintf("\n");
    cliPrintf("%02X ",buffer[i]);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_2)
  {
    int_count ++;
    bno085_tbl.INT_pinstate = true;
  }
}


void bno085Complete(void)
{

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(hspi == bno085_setting_tbl.spi_handler)
  {
    bno085Complete();
  }
}
//GPIO_PinState bno085IntUpdate(void)
//{
//  GPIO_PinState ret = HAL_GPIO_ReadPin(bno085_setting_tbl.INT_Port, bno085_setting_tbl.INT_Pin);
//  if(ret == GPIO_PIN_RESET)bno085_tbl.INT_pinstate = true;
//  return ret;
//}
