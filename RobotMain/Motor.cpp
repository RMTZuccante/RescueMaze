#include "Motor.h"

/**
 * Sets the motor pins.
 * @param inverter1 First inverter pin.
 * @param inverter2 Second inverter pin.
 */
Motor::Motor(uint8_t inverter1, uint8_t inverter2, uint8_t pwm) {
  this->inverter1 = inverter1;
  this->inverter2 = inverter2;
  this->pwm = pwm;
  this->inverse = false;
}

/**
 * Starts the motor rotation.
 * @param speed Speed of the motor, from 0 to 65535.
 * @param inverse Motor turning direction.
 */
void Motor::start(uint16_t speed, bool inverse) {
  this->inverse = inverse;
  digitalWrite(inverter1, !inverse);
  digitalWrite(inverter2, inverse);
  pwmWrite(pwm, speed);
}

/**
 * Makes the motor ready to rotate.
 */
void Motor::begin() {
  pinMode(inverter1, OUTPUT);
  pinMode(inverter2, OUTPUT);
  pinMode(pwm, PWM);
  digitalWrite(inverter1, 0);
  digitalWrite(inverter2, 0);
  pwmWrite(pwm, 0);
}

/**
 * Adjusts the motor speed.
 * @param speed Speed of the motor, from 0 to 65535.
 */
void Motor::setSpeed(uint16_t speed) {
  start(speed, inverse);
}

/**
 * Stops the motor.
 */
void Motor::stop() {
  digitalWrite(inverter1, 1);
  digitalWrite(inverter2, 1);
  pwmWrite(pwm, 0);
}
