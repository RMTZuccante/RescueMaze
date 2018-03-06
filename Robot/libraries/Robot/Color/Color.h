#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "I2C.h"

#define COLORADDRESS 42

class Color {
  public:
    Color(int merror);
    void begin();
    bool check();
    uint8_t read();
  private:
    int merror;
};

#endif
