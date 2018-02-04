#include "Moviment.h"

Moviment::Moviment (uint16_t speed) {
  this->speed = speed;
}

void Moviment::begin(){
  motorFR.begin();
  motorFL.begin();
  motorRR.begin();
  motorRL.begin();
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

void Moviment::setSpeed(uint16_t velocity) {
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
  if (endRotation-direzione>0) setK(FIRST_K+((endRotation - direzione)*500), SECOND_K+((endRotation - direzione)*500));
  else setK(SECOND_K+((direzione - endRotation)*500), FIRST_K+((direzione - endRotation)*500));
  rotate(direction);
}

short Moviment::bound(uint16_t n, uint16_t max) {
  return (n > max) ? max : n;
}
