#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor {
  public:
    Motor(uint8_t inverter1, uint8_t inverter2);
    void start(uint16_t speed, bool inverse);
    void setSpeed(uint16_t speed);
    void stop();
    void begin();
  private:
    uint8_t inverter1, inverter2;
    bool inverse;
};

#endif
