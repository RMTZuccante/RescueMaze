#ifndef Color_h
#define Color_h

#include <Arduino.h>
#include "Debug.h"
#include "APDS9960.h"

struct ColorData {
  uint16_t ambient;
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

class Color {
  public:
    Color(uint8_t black_threshold = 50) : black_threshold(black_threshold) {}
    void begin();
    bool check();
    ColorData read();
    bool isBlack();
  private:
    APDS9960 apds;
    uint8_t black_threshold;
};

#endif
