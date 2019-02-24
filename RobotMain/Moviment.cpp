#include "Moviment.h"
#include "Debug.h"
/**
 * Sets the movement speed and default parameters.
 */
Moviment::Moviment (uint16_t speed, Temperature *tl, Temperature *tr) {
  this->speed = speed;
  tleft = tl;
  tright = tr;
  fill=0;
}

/**
 * Makes the robot ready to move.
 */
void Moviment::begin() {
  motorsR.begin();
  motorsL.begin();
  orientation.begin();
  delay(100);
  orientation.calibrate();
  delay(100);
  orientation.start(100);
}

/**
 * Checks if the sensors are ready to be read from.
 * @return
 */
bool Moviment::check() {
  return orientation.check();
}

/**
 * Sets the motors to high speed to easily climb.
 */
void Moviment::climb(int k) {
  k=-k*CLIMB_K;
  motorsR.start(bound(50000+k,65535), false);
  motorsL.start(bound(50000-k,65535), false);
}

/**
 * Makes the robot IMPENNARSI!!!
 */
void Moviment::impennati(uint16_t speed){
  motorsR.startDiff(0, bound(speed, 65535), false);
  motorsL.startDiff(0, bound(speed, 65535), false);
}
 
/**
 * Moves the robot forward.
 */
void Moviment::go() {
  go(false);
}

/**
 * Moves the robot forward or backward.
 * @param invert Forward if TRUE, backward if FALSE.
 */
void Moviment::go(bool invert) {
  orientation.start(20);
  direzione = orientation.yaw();
  motorsR.start(bound(speed, 65535), invert);
  motorsL.start(bound(speed, 65535), invert);
}

/**
 * Moves the robot forward in a straight line.
 */
void Moviment::straight() {
  float fix = direzione - orientation.yaw();
  motorsR.start(bound(speed - fix * 5000, 65535), false);
  motorsL.start(bound(speed + fix * 5000, 65535), false);
}

/**
 * Straightens the robot at the end of a movement.
 */
void Moviment::endGo() {
  float now = orientation.yaw();
  (direzione > now) ? rotate(true, direzione - now) : rotate(false, now - direzione);
}

/**
 * Rotates the robot by 90 degrees to the right.
 */
void Moviment::rotate() {
  rotate(false);
}

/**
 * Makes the robot rotate until stop is called.
 * @param invert Rotates right if FALSE, left if TRUE.
 */
void Moviment::rotation(bool invert){
  motorsR.start(30000, invert);
  motorsL.start(30000, !invert);
}

/**
 * Rotates the robot by 90 degrees.
 * @param invert Rotates right if FALSE, left if TRUE.
 */
int Moviment::rotate(bool invert) {
  return rotate(invert, 90);
  //delay(1000);
}

/**
 * Rotates the robot by a given number degrees.
 * @param invert Rotates right if FALSE, left if TRUE.
 * @param angle The angle by.
 */
int Moviment::rotate(bool invert , float angle) {
  Debug.println("rotate start");
  orientation.start(100);
  float end = endAngle(orientation.yaw(), invert , angle);
  bool isVictimL = false;
  bool isVictimR = false;
  end-=fill;
  if (!invert) {
    if (end > (360 - angle)) {
      rotationSpeed(ROTATION_SPEED, invert);
      while (orientation.yaw() > 1) {};
      delay(100);
    }
    while (orientation.yaw() > end) {
      rotationSpeed(ROTATION_SPEED, invert);
      isVictimL |= (tleft->read()) > TEMP_K;
      isVictimR |= (tright->read()) > TEMP_K;
    }
    stop();
    while (orientation.yaw() <= (end)) {
      rotationSpeed(!invert, end);
    }
  }
  else {
    if (end < angle) {
      rotationSpeed(ROTATION_SPEED, invert);
      while (orientation.yaw() < 359) {};
      delay(100);
    }
    while (orientation.yaw() < end) {
      rotationSpeed(ROTATION_SPEED, invert);
      isVictimL |= (tleft->read()) > TEMP_K;
      isVictimR |= (tright->read()) > TEMP_K;
    }
    stop();
    while (orientation.yaw() >= (end)) {
      rotationSpeed(!invert, end);
    }
  }
  stop();
  setK(0, 0);
  fill=(orientation.yaw()-end);
  Debug.println("rotate end");
  if(isVictimL)return 1;
  if(isVictimR)return 2;
  return 0;
}

/**
 * Stops the motor.
 */
void Moviment::stop() {
  motorsR.stop();
  motorsL.stop();
}

/**
 * Adjust the speed of the motors.
 * @param speed The speed, from 0 to 65535.
 */
void Moviment::setSpeed(uint16_t speed) {
  this->speed = bound((speed) , 65535);
  motorsR.setSpeed(speed);
  motorsL.setSpeed(speed);
}

/**
 * Set the rotation coefficient.
 * @param rightK Right coefficient.
 * @param leftK Left coefficient.
 */
void Moviment::setK(int rightK, int leftK) {
  kR = rightK;
  kL = leftK;
}

/**
 * Calculates the angle in which the robot has to stop.
 * @param angle Actual angle.
 * @param invert Rotates right if FALSE, left if TRUE.
 * @param end The end angle of the rotation.
 * @return The angle to turn of.
 */
float Moviment::endAngle(float angle, bool invert, float end) {
  float ris = angle;
  if (invert) {
    ris += end;
    return (ris > 360) ? ris - 360 : ris;
  }
  else {
    ris -= end;
    return (ris < 0) ? ris + 360 : ris;
  }
}

/**
 * Adjust the rotation speed, based on the distance from the given angle.
 * @param invert Rotates right if FALSE, left if TRUE.
 * @param endRotation The end angle of the rotation.
 */
void Moviment::rotationSpeed(bool invert, float endRotation) {
  direzione = orientation.yaw();
  if (endRotation - direzione > 0) setK(FIRST_K + ((endRotation - direzione) * ROTATION_P), SECOND_K + ((endRotation - direzione) * ROTATION_P));
  else setK(SECOND_K + ((direzione - endRotation) * ROTATION_P), FIRST_K + ((direzione - endRotation) * ROTATION_P));
  motorsR.start(bound((speed + kR) , 65535), invert);
  motorsL.start(bound((speed + kL) , 65535), !invert);
}

/**
 * Sets the motor rotation speed to the given one.
 * @param speed Speed of the motors, from 0 to 65535.
 * @param invert Rotates right if FALSE, left if TRUE.
 */
void Moviment::rotationSpeed(uint16_t speed, bool invert) {
  motorsR.start(speed, invert);
  motorsL.start(speed, !invert);
}

/**
 * Bound n to max.
 * Cap down n to not overflow the max value.
 * @param n The number given.
 * @param max The max value that n should be.
 * @return The capped value.
 */
uint16_t Moviment::bound(uint32_t n, uint16_t max) {
  return (n > max) ? max : n;
}

/**
 * Reads the inclination from the orientation sensor.
 * @return The already corrected inclination.
 */
float Moviment::inclination() {
  return orientation.inclination();
}

/**
 * Does idle operation, aka does nothing.
 */
void Moviment::idle() {
  orientation.update();
}
