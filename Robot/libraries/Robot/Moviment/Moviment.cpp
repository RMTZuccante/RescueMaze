#include "Moviment.h"

Moviment::Moviment (uint16_t speed) {
  this->speed = speed;
}

void Moviment::begin() {
//  Serial.println("begun");
  motorFR.begin();
  motorFL.begin();
  motorRR.begin();
  motorRL.begin();
  orientation.begin();
  delay(100);
  orientation.calibrate();
  delay(100);
  orientation.start(100);
  
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
  orientation.start(20);
  direzione = orientation.yaw();
  motorFR.start(bound(speed, 65535), !invert);
  motorFL.start(bound(speed, 65535), !invert);
  motorRR.start(bound(speed, 65535), !invert);
  motorRL.start(bound(speed, 65535), !invert);
}

void Moviment::straight(){
  float fix=direzione-orientation.yaw();
  Serial.print(direzione);
  Serial.print("  ");
  Serial.println(orientation.yaw());
  motorFR.start(bound(speed - fix*5000, 65535), false);
  motorFL.start(bound(speed + fix*5000, 65535), false);
  motorRR.start(bound(speed - fix*5000, 65535), false);
  motorRL.start(bound(speed + fix*5000, 65535), false);
}

void Moviment::endGo(){
  float now=orientation.yaw();
//  Serial.println(now);
//  Serial.println(direzione);
  (direzione > now) ? rotate(true,direzione-now):rotate(false,now-direzione); 
}
void Moviment::rotate() {
  rotate(false);
}
void Moviment::rotate(bool invert){
  rotate(invert,90);
}

void Moviment::rotate(bool invert ,float angle) {
//  Serial.println(angle);
//  delay(5000);
  orientation.start(100);
  float end = endAngle(orientation.yaw(), invert , angle); 
  if (!invert) {
    if (end > (360-angle)) {
      rotationSpeed(invert, 0);
      while (orientation.yaw() > 5) {};
      delay(100);
    }
    while (orientation.yaw() > end) {
      rotationSpeed(invert, end);
    }
    stop();
    delay(100);
    while (orientation.yaw() <= (end+FILL_R)) {
      rotationSpeed(!invert, end);
      //delay(500);
    }
  }
  else {
    if (end < angle) {
      rotationSpeed(invert, 360);
      while (orientation.yaw() < 355) {};
      delay(100);
    }
    while (orientation.yaw() < end) {
      rotationSpeed(invert, end);
    }
    stop();
    delay(100);
    while (orientation.yaw() >=(end-FILL_L)) {
      rotationSpeed(!invert, end);
    }
  }
  stop();
  setK(0, 0);
  
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
}

float Moviment::endAngle(float angle, bool invert, float end) {
  float ris=angle;
  if (invert) {
    ris += end;
    return (ris > 360) ? ris - 360 : ris;
  }
  else {
    ris -= end;
//    Serial.println("endangle");
//    Serial.println((ris < 0) ? ris + 360 : ris);
    return (ris < 0) ? ris + 360 : ris;
  }
}

void Moviment::rotationSpeed(bool invert , float endRotation) {
  direzione = orientation.yaw();
  if (endRotation - direzione > 0) setK(FIRST_K + ((endRotation - direzione) * ROTATION_H), SECOND_K + ((endRotation - direzione) * ROTATION_H));
  else setK(SECOND_K + ((direzione - endRotation) * ROTATION_H), FIRST_K + ((direzione - endRotation) * ROTATION_H));
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
