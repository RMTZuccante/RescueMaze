#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "definitions.h"
#include "Debug.h"
#include "I2C.h"

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
