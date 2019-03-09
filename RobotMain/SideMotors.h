#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class SideMotors {
  public:
    SideMotors(uint8_t inverter1, uint8_t inverter2, uint8_t pwmFront, uint8_t pwmRear);
    void startDiff(uint16_t frontSpeed, uint16_t rearSpeed, bool inverse);
    void start(uint16_t speed, bool inverse);
    void setSpeed(uint16_t speed);
    void setSpeeds(uint16_t frontSpeed, uint16_t rearSpeed);
    void stop();
    void begin();
  private:
    uint8_t inverter1, inverter2, pwmFront, pwmRear;
    bool inverse;
};

#endif
