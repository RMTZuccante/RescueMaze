#include "Color.h"

Color::Color(int merror) {
  this->merror = merror;
}

void Color::begin() {
  I2C_1.begin();
  I2C_1.beginTransmission(COLORADDRESS);
  I2C_1.write(10 + merror);
  I2C_1.endTransmission();
}

bool Color::check() {
  I2C_1.beginTransmission(COLORADDRESS);
  if (I2C_1.endTransmission()) return false;
  return true;
}

uint8_t Color::read() {
  I2C_1.requestFrom(COLORADDRESS, 1);
  return I2C_1.read();
}
