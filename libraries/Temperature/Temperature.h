#ifndef Temperature_h
#define Temperature_h

#include <Arduino.h>
#include <Wire.h>

#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07

class Temperature  {
 public:
  Temperature(byte address);
  float read();
  float readAmb();
 private:
  float readTemp(byte reg);
  byte address;
  unsigned short read16(byte reg);
};

#endif
