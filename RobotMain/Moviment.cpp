#include "Moviment.h"
#include "Debug.h"
/**
 * Sets the movement speed and default parameters.
 */
Moviment::Moviment (uint16_t speed, Temperature *tl, Temperature *tr, float *isVictimL,float *isVictimR) {
  this->speed = speed;
  tleft = tl;
  tright = tr;
  this->isVictimL = isVictimL;
  this->isVictimR = isVictimR;
}

/**
 * Makes the robot ready to move.
 */
void Moviment::begin() {
  motorsR.begin();
  motorsL.begin();
  orientation.begin();
}

void Moviment::calibrate() {
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
  motorsR.start(min(50000+k,MAXSPEED), false);
  motorsL.start(min(50000-k,MAXSPEED), false);
}
 
/**
 * Moves the robot forward.
 */
void Moviment::go() {
  orientation.start(20);
  direzione = orientation.yaw();
  go(false);
}

/**
 * Moves the robot forward or backward.
 * @param invert Forward if TRUE, backward if FALSE.
 */
void Moviment::go(bool invert) {
  motorsR.start(min(speed, MAXSPEED), invert);
  motorsL.start(min(speed, MAXSPEED), invert);
}
/**
 * Straightens the robot by checking on the starting direction
 */

void Moviment::straighten() {
  float fix = (direzione - orientation.yaw()) * STR_K;
  fix = fix > 0 ? fix : 0;
  motorsR.start(min(speed + fix, MAXSPEED), false);
  motorsL.start(min(speed - fix, MAXSPEED) , false);
}

/**
 * Straightens the robot at the end of a movement.
 */
void Moviment::endGo() {
  float now = orientation.yaw();
  if (abs(direzione-now) > 2) {
    if (direzione > now) rotate(true, direzione - now,BASIC);
    else rotate(false, now - direzione,BASIC);
  }
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
  motorsR.start(SLOW_ROTATION, invert);
  motorsL.start(SLOW_ROTATION, !invert);
}

/**
 * Rotates the robot by 90 degrees.
 * @param invert Rotates right if FALSE, left if TRUE.
 */
void Moviment::rotate(bool invert) {
  rotate(invert, 90, BASIC);
}

/**
 * Rotates the robot by a given number degrees.
 * @param invert Rotates right if FALSE, left if TRUE.
 * @param angle The angle by.
 */
void Moviment::rotate(bool invert , float angle , byte type) {
  orientation.start(100);
  float end = endAngle(orientation.yaw(), invert , angle);
  if(end==0)end+=1;
  if(end==360)end-=1;
  Debug.println(String("startAngle ")+String(orientation.yaw()));
  Debug.println(String("endAngle ")+String(end));
  Debug.print("left ");
  while (invert ? orientation.yaw() < end : orientation.yaw() > end) {
      //Debug.println(String(orientation.yaw()));
      rotationSpeed(ROTATION_SPEED, invert, type);
      *isVictimL = max(*isVictimL, tleft->read());
      *isVictimR = max(*isVictimR, tright->read());
  }
  stop();
  Debug.println(" Correction");
  while (invert ? orientation.yaw() > end : orientation.yaw() < end) {
    rotationSpeed(!invert, end);
  }
  stop();
  setK(0, 0);
  delayr(50);
  Debug.println("rotate end");
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
  this->speed = min((speed) , MAXSPEED);
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
  motorsR.start(min((SLOW_ROTATION + kR) , MAXSPEED), !invert);
  motorsL.start(min((SLOW_ROTATION + kL) , MAXSPEED), invert);
}

/**
 * Sets the motor rotation speed to the given one.
 * @param speed Speed of the motors, from 0 to 65535.
 * @param invert Rotates right if FALSE, left if TRUE.
 */
void Moviment::rotationSpeed(uint16_t speed, bool invert,byte type) {
  uint16_t speedR=speed;
  uint16_t speedL=speed;
  
  if(type == FRONT_WALL){
    invert?speedL/=2:speedR/=2;
  }
  else if(type == BACK_WALL){
    invert?speedR/=2:speedL/=2;
  }
  motorsR.start(speedR, !invert);
  motorsL.start(speedL, invert);
}

/**
 * Reads the inclination from the orientation sensor.
 * @return The already corrected inclination.
 */
float Moviment::inclination() {
  return orientation.inclination();
}

 float Moviment::getRoll(){
  return orientation.roll();
 }

float Moviment::getTemp() {
  return orientation.getTemperature();
}
 
/**
 * Does idle operation, aka does nothing.
 */
void Moviment::idle() {
  orientation.update();
}
/**
 * Idles for the given time.
 * Instead of doing nothing this function keep updating filters.
 * @param t Time in milliseconds to wait.
 */
void Moviment::delayr(unsigned int t) {
  unsigned int end = millis() + t;
  while (end > millis()) idle();
}
