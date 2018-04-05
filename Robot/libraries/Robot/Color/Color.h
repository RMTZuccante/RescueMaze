#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "I2C.h"

#define COLORADDRESS 0x42

class Color {
  public:
    Color(byte merror, byte mcolor);
    void begin();
    bool check();
    uint8_t read();
    uint8_t readIR();
    uint8_t readColor();
  private:
    uint8_t read(uint8_t data);
    byte merr;
    byte mcol;
};

#endif
