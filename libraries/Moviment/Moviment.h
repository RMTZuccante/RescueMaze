#ifndef Moviment_h
#define Moviment_h

#include "Motor.h"

#define FR_EN 10
#define FR_IN1 8
#define FR_IN2 11
#define FL_EN 5
#define FL_IN1 2
#define FL_IN2 3
#define RR_EN 9
#define RR_IN1 12
#define RR_IN2 13
#define RL_EN 6
#define RL_IN1 7
#define RL_IN2 4

class Moviment {
  public:
    Moviment (byte speed);
    void go();
    void go (bool invert);
    void rotate();
    void rotate (bool invert);
    void stop();
    void setSpeed(byte speed);
    void setK(int rightK, int leftK);
  private:
    short bound (short n, short max);
    Motor motorFR = Motor(FL_EN, FL_IN1, FL_IN2);
    Motor motorFL = Motor(FR_EN, FR_IN1, FR_IN2);
    Motor motorRR = Motor(RL_EN, RL_IN1, RL_IN2);
    Motor motorRL = Motor(RR_EN, RR_IN1, RR_IN2);
	byte state;
    byte speed;
    int kR;
    int kL;
};

Moviment::Moviment (byte velocity) {
  state=0;
  speed = velocity;
}

void Moviment::go() {
  go(false);
}

void Moviment::go(bool invert) {
  motorFR.start(bound(speed + kR, 255), !invert);
  motorFL.start(bound(speed + kL, 255), !invert);
  motorRR.start(bound(speed + kR, 255), !invert);
  motorRL.start(bound(speed + kL, 255), !invert);
}

void Moviment::rotate() {
  rotate(false);
}

void Moviment::rotate(bool invert) {
  motorFR.start(bound((speed + kR) , 255), !invert);
  motorFL.start(bound((speed + kL) , 255), invert);
  motorRR.start(bound((speed + kR) , 255), !invert);
  motorRL.start(bound((speed + kL) , 255), invert);
}

void Moviment::stop() {
  motorFR.stop();
  motorFL.stop();
  motorRR.stop();
  motorRL.stop();
}

void Moviment::setSpeed(byte velocity) {
  speed = velocity;
  motorFR.setSpeed(velocity);
  motorFL.setSpeed(velocity);
  motorRR.setSpeed(velocity);
  motorRL.setSpeed(velocity);
}

void Moviment::setK(int rightK, int leftK) {
  kR = rightK;
  kL = leftK;
  motorFR.setSpeed(bound((speed + kR) , 255));
  motorFL.setSpeed(bound((speed + kL) , 255));
  motorRR.setSpeed(bound((speed + kR) , 255));
  motorRL.setSpeed(bound((speed + kL) , 255));
}

short Moviment::bound(short n, short max) {
  return (n > max) ? max : n;
}

#endif
