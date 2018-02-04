#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor {
  public:
    Motor(byte i1, byte i2);
    void start(uint16_t speed, bool inverse);
    void setSpeed(uint16_t speed);
    void stop();
    void begin();
  private:
    byte inverter1, inverter2;
    bool inverse;
};

#endif
