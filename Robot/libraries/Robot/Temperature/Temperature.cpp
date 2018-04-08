#include "Temperature.h"

/**
 * Sets the sensors parameters.
 * @param address Address of the sensor.
 */
Temperature::Temperature(uint8_t address) {
  this->address = address;
}

/**
 * Check if the sensor is connected and answers.
 * @return TRUE if the sensor is there.
 */
bool Temperature::check() {
  I2C_1.beginTransmission(address);
  return !I2C_1.endTransmission();
}

/**
 * Reads the actual object temperature.
 */
float Temperature::read() {
  return readTemp(MLX90614_TOBJ1);
}

/**
 * Reads the actual sensors inside temperature.
 */
float Temperature::readAmb() {
  return readTemp(MLX90614_TA);
}

/**
 * Reads a temperature by its register.
 * @param reg The register to be read.
 * @return The already corrected temperature.
 */
float Temperature::readTemp(uint8_t reg) {
  return read16(reg) * 0.02f - 273.15f;
}

/**
 * Reads a 2 byte register.
 * @param reg Register to be read.
 * @return The raw value read.
 */
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
