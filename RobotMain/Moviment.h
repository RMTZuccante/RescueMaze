#ifndef Moviment_h
#define Moviment_h

#include <Arduino.h>
#include "definitions.h"
#include "Motor.h"
#include "Temperature.h"
#include "IMU.h"

class Moviment {
  public:
    Moviment (uint16_t speed, Temperature *tl, Temperature *tr);
    void begin();
    bool check();
    void go();
    void go (bool invert);
    void impennati(uint16_t speed);    
    void straight();
    void endGo();
    void rotate();
    void climb(int k);
    void rotation(bool invert);
    int rotate (bool invert);
    int rotate (bool invert, float angle);
    void stop();
    void setSpeed(uint16_t speed);
    void setK(int rightK, int leftK);
    float inclination();
    void idle();
  private:
    uint16_t bound (uint32_t n, uint16_t max);
    float endAngle(float angle, bool invert, float end);
    void rotationSpeed(bool direction , float endRotation);
    void rotationSpeed(uint16_t speed, bool invert);
    Motor motorFR = Motor(FL_IN1, FL_IN2, FL_EN);
    Motor motorFL = Motor(FR_IN1, FR_IN2, FR_EN);
    Motor motorRR = Motor(RL_IN1, RL_IN2, RL_EN);
    Motor motorRL = Motor(RR_IN1, RR_IN2, RR_EN);
    IMU orientation;
    Temperature *tright;
    Temperature *tleft;
    uint16_t speed;
    int kR;
    int kL;
    float direzione;
    float fill;
};

#endif