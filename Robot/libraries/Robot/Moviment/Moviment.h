#ifndef Moviment_h
#define Moviment_h

#include <Arduino.h>
#include "definitions.h"
#include "Motor.h"
#include "IMU.h"

class Moviment {
  public:
    Moviment (uint16_t speed);
    void begin();
    bool check();
    void go();
    void go (bool invert);
    void straight();
    void endGo();
    void rotate();
    void climb();
    void rotate (bool invert);
    void rotate (bool invert, float angle);
    void stop();
    void setSpeed(uint16_t speed);
    void setK(int rightK, int leftK);
    float getPitch();
    void idle();
  private:
    uint16_t bound (uint32_t n, uint16_t max);
    float endAngle(float angle, bool invert, float end);
    void rotationSpeed(bool direction , float endRotation);
    void rotationSpeed(uint16_t speed, bool invert);
    Motor motorFR = Motor(FL_IN1, FL_IN2);
    Motor motorFL = Motor(FR_IN1, FR_IN2);
    Motor motorRR = Motor(RL_IN1, RL_IN2);
    Motor motorRL = Motor(RR_IN1, RR_IN2);
    IMU orientation;
    uint16_t speed;
    int kR;
    int kL;
    float direzione;
    float fill;
};

#endif
