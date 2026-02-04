/*
 * bno085_i2c.c
 *
 *  Created on: 2026. 2. 4.
 *      Author: jojae
 */


#ifdef BNO_I2C

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

#endif
