#include "Motor.h"

Motor::Motor(byte i1, byte i2) {
  inverter1 = i1;
  inverter2 = i2;
}

void Motor::start(uint16_t speed, bool inverse) {
  this->inverse = inverse;
  pwmWrite(inverter1, inverse? 0 : speed);
  pwmWrite(inverter2, inverse? speed : 0);
}

void Motor::begin() {
  pinMode(inverter1, PWM);
  pinMode(inverter2, PWM);
  pwmWrite(inverter1, 0);
  pwmWrite(inverter2, 0);
}

void Motor::setSpeed(uint16_t speed) {
  start(speed, inverse);
}

void Motor::stop() {
  pwmWrite(inverter1, 0);
  pwmWrite(inverter2, 0);
}
