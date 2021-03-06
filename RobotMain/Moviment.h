#ifndef Moviment_h
#define Moviment_h

#include <Arduino.h>
#include "definitions.h"
#include "Temperature.h"
#include "IMU.h"
#include "SideMotors.h"

class Moviment {
  public:
    Moviment (uint16_t speed, Temperature *tl, Temperature *tr , float *isVictimL,float *isVictimR);
    void begin();
    void calibrate();
    bool check();
    void go();
    void go (bool invert);
    void endGo();
    void rotate();
    void climb(int k);
    void rotation(bool invert);
    void rotate (bool invert);
    void rotate (bool invert, float angle, byte type);
    void stop();
    void setSpeed(uint16_t speed);
    void setK(int rightK, int leftK);
    float inclination();
    float getRoll();
    float getTemp();
    void idle();
    void delayr(unsigned int t);
    void straighten();
  private:
    float endAngle(float angle, bool invert, float end);
    void rotationSpeed(bool direction , float endRotation);
    void rotationSpeed(uint16_t speed, bool invert,byte type);
    SideMotors motorsR = SideMotors(FR_IN1, FR_IN2, FR_EN, RR_EN);
    SideMotors motorsL = SideMotors(FL_IN1, FL_IN2, FL_EN, RL_EN);
    IMU orientation;
    Temperature *tright;
    Temperature *tleft;
    float *isVictimL;
    float *isVictimR;
    uint16_t speed;
    int kR;
    int kL;
    float direzione;
};

#endif
