#include "Temperature.h"

Temperature::Temperature(uint8_t address) {
  this->address = address;
}

bool Temperature::check() {
  I2C_1.beginTransmission(address);
  if (I2C_1.endTransmission()) return false;
  return true;
}

float Temperature::read() {
  return readTemp(MLX90614_TOBJ1);
}

float Temperature::readAmb() {
  return readTemp(MLX90614_TA);
}

float Temperature::readTemp(uint8_t reg) {
  return read16(reg) * 0.02 - 273.15;
}
uint16_t Temperature::read16(uint8_t reg) {
  unsigned short raw;
  I2C_1.beginTransmission(address);
  I2C_1.write(reg);
  I2C_1.endTransmission(false);
  I2C_1.requestFrom(address, (uint8_t)3);
  raw = I2C_1.read();
  raw |= I2C_1.read() << 8;
  uint8_t pec = I2C_1.read();
  return raw;
}
