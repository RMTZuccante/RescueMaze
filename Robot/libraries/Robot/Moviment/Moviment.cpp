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

void Moviment::climb(){
  motorFR.start(50000, false);
  motorFL.start(50000, false);
  motorRR.start(20000, false);
  motorRL.start(20000, false);
}
void Moviment::go() {
  go(true);
}

void Moviment::go(bool invert) {
  //direzione = orientation.yaw();
  motorFR.start(bound(speed, 65535), !invert);
  motorFL.start(bound(speed, 65535), !invert);
  motorRR.start(bound(speed, 65535), !invert);
  motorRL.start(bound(speed, 65535), !invert);
}

//void Moviment::straight(){
//  motorFR.start(bound(speed + (direzione-orientation.yaw())*1000, 65535), false);
//  motorFL.start(bound(speed - (direzione-orientation.yaw())*1000, 65535), false);
//  motorRR.start(bound(speed + (direzione-orientation.yaw())*1000, 65535), false);
//  motorRL.start(bound(speed - (direzione-orientation.yaw())*1000, 65535), false);
//}

void Moviment::rotate() {
  rotate(false);
}

void Moviment::rotate(bool invert) {
  orientation.start();
  float end = endAngle(orientation.yaw(), invert); 
  if (!invert) {
    if (end > 270) {
      rotationSpeed(invert, 0);
      while (orientation.yaw() > 5) {};
      delay(100);
    }
    while (orientation.yaw() > end) {
      rotationSpeed(invert, end);
    }
    stop();
    while (orientation.yaw() < end) {
      rotationSpeed(!invert, end-10);
      //delay(500);
    }
  }
  else {
    if (end < 90) {
      rotationSpeed(invert, 360);
      while (orientation.yaw() < 355) {};
      delay(100);
    }
    while (orientation.yaw() < end) {
      rotationSpeed(invert, end);
    }
    stop();
    while (orientation.yaw() > end) {
      rotationSpeed(!invert, end+10);
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
  this->speed = bound((speed) , 65535);
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
  float ris=angle;
  if (invert) {
    ris += 90;
    return (ris > 360) ? ris - 360 : ris;
  }
  else {
    ris -= 90;
    Serial.println("endangle");
    Serial.println((ris < 0) ? ris + 360 : ris);
    return (ris < 0) ? ris + 360 : ris;
  }
}

void Moviment::rotationSpeed(bool invert , float endRotation) {
  direzione = orientation.yaw();
  if (endRotation - direzione > 0) setK(FIRST_K + ((endRotation - direzione) * 200), SECOND_K + ((endRotation - direzione) * 200));
  else setK(SECOND_K + ((direzione - endRotation) * 200), FIRST_K + ((direzione - endRotation) * 200));
  motorFR.start(bound((speed + kR) , 65535), invert);
  motorFL.start(bound((speed + kL) , 65535), !invert);
  motorRR.start(bound((speed + kR*RK) , 65535), invert);
  motorRL.start(bound((speed + kL*RK) , 65535), !invert);
  Serial.print(endRotation);
  Serial.print("  ");
  Serial.println(direzione);
}

uint16_t Moviment::bound(uint32_t n, uint16_t max) {
  return (n > max) ? max : n;
}

float Moviment::getPitch() {
  return orientation.pitch();
}
