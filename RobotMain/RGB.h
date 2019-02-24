#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

class RGB {
  public:
    RGB(uint8_t pinr, uint8_t ping, uint8_t pinb);
    void set(bool red, bool green, bool blue);
    void begin();
  private:
    uint8_t pin[3];
};


#endif
