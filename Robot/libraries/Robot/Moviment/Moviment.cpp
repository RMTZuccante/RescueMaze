#include "Moviment.h"

Moviment::Moviment (uint16_t speed) {
  this->speed = speed;
}

void Moviment::begin() {
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

bool Moviment::check() {
  return orientation.check();
}

void Moviment::go() {
  go(true);
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
  float end = endAngle(orientation.yaw(), invert);
  if (invert) {
    if (end < 90) {
      rotationSpeed(invert, 0);
      while (orientation.yaw() < 355) {};
      delay(100);
    }
    while (orientation.yaw() < end) {
      rotationSpeed(invert, end);
    }
    stop();
    while (orientation.yaw() > end) {
      rotationSpeed(!invert, end);
    }
  }
  else {
    if (end < 270) {
      rotationSpeed(!invert, 360);
      while (orientation.yaw() > 5) {};
      delay(100);
    }
    while (orientation.yaw() > end) {
      rotationSpeed(!invert, end);
    }
    stop();
    while (orientation.yaw() < end) {
      rotationSpeed(invert, end);
    }
  }
  setK(0, 0);
  stop();
}

void Moviment::stop() {
  motorFR.stop();
  motorFL.stop();
  motorRR.stop();
  motorRL.stop();
}

void Moviment::setSpeed(uint16_t speed) {
  this->speed = speed;
  motorFR.setSpeed(speed);
  motorFL.setSpeed(speed);
  motorRR.setSpeed(speed);
  motorRL.setSpeed(speed);
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

void Moviment::rotationSpeed(bool invert , float endRotation) {
  direzione = orientation.yaw();
  if (endRotation - direzione > 0) setK(FIRST_K + ((endRotation - direzione) * 200), SECOND_K + ((endRotation - direzione) * 200));
  else setK(SECOND_K + ((direzione - endRotation) * 200), FIRST_K + ((direzione - endRotation) * 200));
  motorFR.start(bound((speed + kR) , 65535), !invert);
  motorFL.start(bound((speed + kL) , 65535), invert);
  motorRR.start(bound((speed + kR) , 65535), !invert);
  motorRL.start(bound((speed + kL) , 65535), invert);
  //  Serial.print(endRotation);
  //  Serial.print("  ");
  //  Serial.print(direzione);
  //  Serial.print(" R: ");
  //  Serial.print(bound((kR) , 65535));
  //  Serial.print(" L: ");
  //  Serial.println(bound((kL) , 65535));
}

uint16_t Moviment::bound(uint32_t n, uint16_t max) {
  return (n > max) ? max : n;
}

float Moviment::getPitch() {
  return orientation.pitch();
}
