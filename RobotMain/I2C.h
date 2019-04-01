#include <Wire.h>
#include <SoftWire.h>

#ifndef I2C_h
#define I2C_h

#include "Debug.h"

class I2C {
  public:
    static void scan(WireBase *i2c);
    static bool check(WireBase *i2c, uint8_t device);
};

extern SoftWire I2C_1;
extern SoftWire I2C_2;

#endif
