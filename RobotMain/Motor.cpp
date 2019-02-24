#include "Motor.h"

/**
 * Sets the motor pins.
 * @param inverter1 First inverter pin.
 * @param inverter2 Second inverter pin.
 */
Motor::Motor(uint8_t inverter1, uint8_t inverter2) {
  this->inverter1 = inverter1;
  this->inverter2 = inverter2;
}

/**
 * Starts the motor rotation.
 * @param speed Speed of the motor, from 0 to 65535.
 * @param inverse Motor turning direction.
 */
void Motor::start(uint16_t speed, bool inverse) {
  this->inverse = inverse;
  pwmWrite(inverter1, inverse ? 0 : speed);
  pwmWrite(inverter2, inverse ? speed : 0);
}

/**
 * Makes the motor ready to rotate.
 */
void Motor::begin() {
  pinMode(inverter1, PWM);
  pinMode(inverter2, PWM);
  pwmWrite(inverter1, 0);
  pwmWrite(inverter2, 0);
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
  pwmWrite(inverter1, 0);
  pwmWrite(inverter2, 0);
}
