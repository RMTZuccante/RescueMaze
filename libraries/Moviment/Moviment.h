#ifndef Moviment_h
#define Moviment_h

#include "Motor.h"
#include "IMU.h"

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
    float endAngle(float angle, bool invert);
    IMU orientation;
    byte state;
    byte speed;
    int kR;
    int kL;
    float direzione;
};

Moviment::Moviment (byte velocity) {
  state=0;
  speed = velocity;
  orientation.begin();
  delay(100);
  orientation.calibrate();
  delay(100);
  orientation.start();
}

void Moviment::go() {
  go(false);
}

void Moviment::go(bool invert) {
  motorFR.start(bound(speed + kR, 65535), !invert);
  motorFL.start(bound(speed + kL, 65535), !invert);
  motorRR.start(bound(speed + kR, 65535), !invert);
  motorRL.start(bound(speed + kL, 65535), !invert);
  
}

void Moviment::rotate() {
  rotate(false);
}

void Moviment::rotate(bool invert) {
  orientation.start();
  motorFR.start(bound((speed + kR) , 65535), !invert);
  motorFL.start(bound((speed + kL) , 65535), invert);
  motorRR.start(bound((speed + kR) , 65535), !invert);
  motorRL.start(bound((speed + kL) , 65535), invert);
  float end = endAngle(orientation.yaw(), invert);
  if (invert) {
    while(orientation.yaw()<end) rotationSpeed(invert,end);
    stop();
    while(orientation.yaw()>end) rotationSpeed(!invert,end);
  }
  else {
    while(orientation.yaw()>end) rotationSpeed(invert,end);
    stop();
    while(orientation.yaw()<end) rotationSpeed(!invert,end);
  }
  setK(0,0);
  stop();
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
  motorFR.setSpeed(bound((speed + kR) , 65535));
  motorFL.setSpeed(bound((speed + kL) , 65535));
  motorRR.setSpeed(bound((speed + kR) , 65535));
  motorRL.setSpeed(bound((speed + kL) , 65535));
}

float Moviment::endAngle(float angle, bool invert) {
  if (invert) {
    angle += 90;
    return (angle > 360) ? angle - 360 : angle;
  }
  else {
    angle -= 90;
    return (angle < 0) ? angle + 360 : angle;
  }
}

void Moviment::rotationSpeed(bool direction , float endRotation) {
  direzione = orientation.yaw();
  if (endRotation-direzione>0) setK(40+((endRotation - direzione)*2), 60+((endRotation - direzione)*2));
  else setK(60+((direzione - endRotation)*2), 40+((direzione - endRotation)*2));
  rotate(direction);
}

short Moviment::bound(short n, short max) {
  return (n > max) ? max : n;
}

#endif
