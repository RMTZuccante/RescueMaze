#ifndef Moviment_h
#define Moviment_h

#include "Motor.h"
#include "IMU.h"
#include <Arduino.h>

#define FIRST_K 6000
#define SECOND_K 4000
#define FR_IN1 PA0
#define FR_IN2 PA1
#define FL_IN1 PA3
#define FL_IN2 PA2
#define RR_IN1 PB0
#define RR_IN2 PB1
#define RL_IN1 PA7
#define RL_IN2 PA6

class Moviment {
  public:
    Moviment (uint16_t speed);
    void begin();
    void go();
    void go (bool invert);
    void rotate();
    void rotate (bool invert);
    void stop();
    void setSpeed(uint16_t speed);
    void setK(int rightK, int leftK);
  private:
    uint16_t bound (uint32_t n, uint16_t max);
    float endAngle(float angle, bool invert);
    void rotationSpeed(bool direction , float endRotation);
    
    Motor motorFR = Motor(FL_IN1, FL_IN2);
    Motor motorFL = Motor(FR_IN1, FR_IN2);
    Motor motorRR = Motor(RL_IN1, RL_IN2);
    Motor motorRL = Motor(RR_IN1, RR_IN2);
    IMU orientation;
    uint16_t speed;
    int kR;
    int kL;
    float direzione;
};

#endif
