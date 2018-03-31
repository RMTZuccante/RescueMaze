#include "Color.h"

/**
 * Sets the color sensor parameters.
 * @param merror Mirror detection threshold.
 * @param mcolor Color detection threshold.
 */
Color::Color(byte merror, byte mcolor) {
  merr = merror;
  mcol = mcolor;
}

/**
 * Starts the sensor and makes it ready to be read.
 */
void Color::begin() {
  I2C_1.begin();
  I2C_1.beginTransmission(COLORADDRESS);
  I2C_1.write(10 + merr);
  I2C_1.write(mcol);
  I2C_1.endTransmission();
}

/**
 * Check if the sensor is working.
 * @return TRUE if it works.
 */
bool Color::check() {
  I2C_1.beginTransmission(COLORADDRESS);
  return !I2C_1.endTransmission();
}
/**
 * Reads the actual color read from the sensors.
 * @return 0 if it detects a white cell, 1 for mirror and 2 for black.
 */
uint8_t Color::read() {
  I2C_1.requestFrom(COLORADDRESS, 1);
  return I2C_1.read();
}
