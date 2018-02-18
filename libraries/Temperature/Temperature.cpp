#inclde "Temperature.h"

Temperature::Temperature(byte address) {
  this->address = address;
}

float Temperature::read() {
  return readTemp(MLX90614_TOBJ1);
}

float Temperature::readAmb() {
  return readTemp(MLX90614_TA);
}

float Temperature::readTemp(byte reg) {
  return read16(reg) * 0.02 - 273.15;
}

unsigned short Temperature::read16(byte reg) {
  unsigned short raw;
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, (byte)3);
  raw = Wire.read();
  raw |= Wire.read() << 8;
  byte pec = Wire.read();
  return raw;
}