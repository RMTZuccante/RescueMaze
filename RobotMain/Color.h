#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "Debug.h"
#include "APDS9960.h"

class Color {
  public:
    void begin();
    bool check();
    uint8_t read();
  private:
    APDS9960 apds;
};

#endif
