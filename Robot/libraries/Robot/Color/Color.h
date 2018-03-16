#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "I2C.h"

#define COLORADDRESS 42

class Color {
  public:
    Color(byte merror, byte mcolor);
    void begin();
    bool check();
    uint8_t read();
  private:
    byte merr;
    byte mcol;
};

#endif
