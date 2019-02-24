#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "Debug.h"
#include "APDS9960.h"

class Color {
  public:
    Color();
    void begin();
    bool check();
    uint8_t read();
  private:
    APDS9960 apds;
    uint16_t ambient_light;
    uint16_t red_light;
    uint16_t green_light;
    uint16_t blue_light;
};

#endif
