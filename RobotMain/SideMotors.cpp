#include "SideMotors.h"

/**
 * Sets the motor pins.
 * @param inverter1 First inverter pin.
 * @param inverter2 Second inverter pin.
 * @param pwmFront 
 * @param pwmRear 
 */
SideMotors::SideMotors(uint8_t inverter1, uint8_t inverter2, uint8_t pwmFront, uint8_t pwmRear) {
  this->inverter1 = inverter1;
  this->inverter2 = inverter2;
  this->pwmFront = pwmFront;
  this->pwmRear = pwmRear;
  this->inverse = false;
}

/**
 * Starts the motor rotation.
 * @param frontSpeed Speed of the front motor, from 0 to 65535.
 * @param rearSpeed Speed of the rear motor, from 0 to 65535.
 * @param inverse Motor turning direction.
 */
void SideMotors::startDiff(uint16_t frontSpeed, uint16_t rearSpeed, bool inverse) {
  this->inverse = inverse;
  digitalWrite(inverter1, !inverse);
  digitalWrite(inverter2, inverse);
  pwmWrite(pwmFront, frontSpeed);
  pwmWrite(pwmRear, rearSpeed);
}

/**
 * Starts the motor rotation.
 * @param speed Speed of the motor, from 0 to 65535.
 * @param inverse Motor turning direction.
 */
void SideMotors::start(uint16_t speed, bool inverse) {
  startDiff(speed, speed, inverse);
}

/**
 * Makes the motor ready to rotate.
 */
void SideMotors::begin() {
  pinMode(inverter1, OUTPUT);
  pinMode(inverter2, OUTPUT);
  pinMode(pwmRear, PWM);
  pinMode(pwmFront, PWM);
  digitalWrite(inverter1, 0);
  digitalWrite(inverter2, 0);
  pwmWrite(pwmRear, 0);
  pwmWrite(pwmFront, 0);
}

/**
 * Adjusts the motor speed.
 * @param frontSpeed Speed of the front motor, from 0 to 65535.
 * @param rearSpeed Speed of the rear motor, from 0 to 65535.
 */
void SideMotors::setSpeeds(uint16_t frontSpeed, uint16_t rearSpeed) {
  startDiff(frontSpeed, rearSpeed, inverse);
}

/**
 * Adjusts the motor speed.
 * @param speed Speed of the motor, from 0 to 65535.
 */
void SideMotors::setSpeed(uint16_t speed) {
  start(speed, inverse);
}

/**
 * Stops the motor.
 */
void SideMotors::stop() {
  digitalWrite(inverter1, 1);
  digitalWrite(inverter2, 1);
  pwmWrite(pwmRear, 0);
  pwmWrite(pwmFront, 0);
}
