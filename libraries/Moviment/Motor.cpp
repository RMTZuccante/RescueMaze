#include "Motor.h"

Motor::Motor(byte i1, byte i2) {
  inverter1 = i1;
  inverter2 = i2;
}

void Motor::start(byte speed, bool inverse) {
  this->inverse = inverse;
  pwmWrite(inverter1, (!inverse)&speed);
  pwmWrite(inverter2, inverse&speed);
}

void Motor::begin() {
  pinMode(inverter1, PWM);
  pinMode(inverter2, PWM);
  pwmWrite(inverter1, 0);
  pwmWrite(inverter2, 0);
}

void Motor::setSpeed(byte speed) {
  start(speed, inverse);
}

void Motor::stop() {
  pwmWrite(inverter1, 0);
  pwmWrite(inverter2, 0);
}
