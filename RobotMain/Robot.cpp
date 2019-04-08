#include "Robot.h"

/**
 * Sets sensors to be used.
 * This function has to be called before doing everything else or the robot will crash.
 */
void Robot::setup() {
  setAddresses();
}

/**
 * Begins all sensors.
 */
void Robot::begin() {
  led.begin();
  color.begin();
  caga.begin();
  mov.begin();
  distances.begin();
  //distances.start();
  //tempk = (temps.left.read()+temps.left.read())/2+TEMP_DIF;
}

void Robot::climb(int k) {
  Debug.println(String("climb"), Levels::INFO);
  mov.climb(k);
}

/**
 * Check if every sensor is connected and working.
 * @return TRUE if everything is ok
 */
bool Robot::check() {
  return temps.check() && color.check() && mov.check() && distances.check();
}

/**
 * Check if the battery is charged enough for the robot to work
 * @return TRUE if the battery voltage is higher than 11 volts
 */
bool Robot::checkBattery() {
  float battery = getBattery();
  bool ok = getBattery()>11.f;
  Debug.println(String("Battery voltage: ")+battery+"v.",Levels::INFO);
  if(!ok) Debug.println("Critical battery level!", Levels::WARN);
  return ok;
}

uint16_t Robot::getDistance(int sensor) {
  switch (sensor) {
    case LASER_FL:
      return distances.frontL.read();
      break;
    case LASER_FR:
      return distances.frontR.read();
      break;
    case LASER_R:
      return distances.right.read();
      break;
    case LASER_L:
      return distances.left.read();
      break;
    case LASER_B:
      return distances.back.read();
      break;
  }
}

Distances Robot::getDistances() {
  return distances;
}

ColorData Robot::getColor() {
	return color.read();
}

void Robot::setBlackThreshold(uint8_t black_threshold) {
  color.setBlack(black_threshold);
}

float Robot::getTempLeft() {
	float out = max(temps.left.read(), isVictimL);
	isVictimL = 0;
	return out;
}

float Robot::getTempRight() {
	float out = max(temps.right.read(), isVictimR);
	isVictimR = 0;
	return out;
}

Temps Robot::getTemps() {
  return temps;
}

int Robot::go(){
  uint16_t zero=distances.frontL.read();
  uint16_t three=distances.frontR.read();
  uint16_t four=distances.back.read()+CELL_DIM;
  return go((zero>three?three:zero)<four);
}

/**
 * Goes forward by 30 cm.
 * Tries to go forward by 30 cm, but stops and goes back if a black cell is detected.
 * @return FALSE if it found a black cell
 */
int Robot::go(bool frontLaser) {
  VL53L0X* dist = &(frontLaser?((distances.frontL.read()<distances.frontR.read()) ? distances.frontL : distances.frontR ) : distances.back);
  uint16_t end = endDist(dist->read(),frontLaser); // calcolo a che distanza devo arrivare
  Debug.println(String("End")+String(end));
  Debug.println(String(frontLaser?"using front laser":"using back laser"));
  int res = 0;
  int i = 0;
  int salita = 0;
  uint16_t front = dist->read();
  uint16_t before = front;
  if(color.isBlack()) res=BLACK; // controllo il colore
  uint16_t start = front;
  Debug.println(String(res));
  mov.go(); // inizio a muovermi
  Debug.println(String("start go "));
  Debug.println(String("First read: ")+front);
  while (((frontLaser) ? (front > end) : (front < end)) && res!=BLACK) {
    // ogni 20 iterazioni controllo l'ostacolo
    if (i == 20) {
      uint16_t now = dist->read();
      if (((before > now) ? before - now : now - before) < 5) {
        mov.charge();
        while(((before > now) ? before - now : now - before) < 10){
          res = OBSTACLE;
          now = dist->read();
        }
      }
      before = now;
      i = 0;
    }
    i++;
    front = dist->read();
    //Debug.println(String("Laser read: ")+front);
    mov.setSpeed(((front - end) * 10) + SPEED);

    // controllo salita
    float incl = mov.inclination();
    if (abs(incl) > RISEINCL) salita++;
    else salita = 0;

    // se rilevata eseguo salita
    if(salita >= 5){
      Debug.println("salita");
      res = RISE;
      while(abs(mov.inclination()) > RISEINCL);
      bool front = (distances.frontL.read()<2000);
      dist = &(front?((distances.frontL.read()<distances.frontR.read()) ? distances.frontL : distances.frontR ) : distances.back);

      end = endDist(dist->read(),front);
    }

    // controllo colore
    if(color.isBlack()) res=BLACK;

    // controllo temperature
    if(abs(start-front) > CENTRED){
      isVictimL = max(isVictimL, temps.left.read());
      isVictimL = max(isVictimR, temps.right.read());
    }
    mov.idle();
  }
  mov.stop();

  // se rilevato nero torno indietro
  if(res==BLACK){
    Debug.println(String("Call to back"));
    back();
  }
  else mov.endGo();
  mov.stop();

  Debug.println(String("stop"));
  straighten();
  if(res!=OBSTACLE){
    //center();
  }
  return res;
}

/**
 * Goes back to the center of the cell.
 */
void Robot::back() {
  back(CELL_DIM);
}
/**
 * Goes back
 * @param length The distance the robot has to back in millimeters
 */
void Robot::back(uint16_t length){
  bool front = (distances.frontL.read()<2000);
  VL53L0X* dist = &(front?((distances.frontL.read()<distances.frontR.read()) ? distances.frontL : distances.frontR ) : distances.back);

  Debug.println(String("back"));
  uint16_t end = endDist(dist->read(), front) + length;
  Debug.println(String("end ") + String(end));
  mov.go(true);
  uint16_t distance = dist->read();
  if (front) while (dist->read() < end) distance = dist->read();
  else while (dist->read() > end)distance = dist->read();
  mov.stop();
}

/**
 * Rotates the robot
 * @param angle degrees to rotate
 * @param dir Rotates right if FALSE, left if TRUE
 */
void Robot::rotate(bool dir, float angle) {
  byte type=BASIC;
  Debug.println(String("Back: ")+String(distances.back.read()));
  Debug.println(String("Front: ")+String(distances.frontL.read()));
  switch(mov.rotate(dir, angle, type)){
    case 1:
      isVictimL=50;
      break;
    case 2:
      isVictimR=50;
      break;
  }
  straighten();
}

/**
 * Rotates the robot by 90 degrees.
 * @param dir Rotates right if FALSE, left if TRUE.
 */
void Robot::rotate(bool dir) {
	rotate(dir, 90);
}

/**
 * Rotates the robot by a given number of degrees
 * @param angle degrees to rotate. Can be negative to rotate left
 */
void Robot::rotate(float angle) {
  rotate(angle<0, abs(angle)); 
}

/**
 * Signals if a victim has been found using led and cagamattoni.
 */
void Robot::victim(int n) {
  Debug.println("Victim found!");
  led.set(LOW, LOW, HIGH);

  int time = millis()+5000;

  if (!caga.isEmpty()) {
    for(int i = 0; i < n; i++) {
      caga.caga();
      mov.delayr(50);
    }
  }
  else {
    for(int i = 0; i < 5; i++) {
      led.set(LOW, LOW, HIGH);
      mov.delayr(500);
      led.set(LOW, LOW, LOW);
      mov.delayr(500);
    }
  }

  while(millis() < time);
  led.set(LOW, LOW, LOW);
}

/**
 * Sets the RGB led.
 * @param red Intensity of the red led
 * @param green Intensity of the green led
 * @param blue Intensity of the blue led
 */
void Robot::setLED(bool red, bool green, bool blue) {
  led.set(red, green, blue);
}

/**
 * Sets addresses to distance sensors.
 */
void Robot::setAddresses() {
  // initializing ToF sensors pins
  pinMode(LX_LEFT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_RIGHT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONTR, OUTPUT_OPEN_DRAIN);
  pinMode(LX_BACK, OUTPUT_OPEN_DRAIN);
  // turning off every ToF sensor
  digitalWrite(LX_LEFT, LOW);
  digitalWrite(LX_RIGHT, LOW);
  digitalWrite(LX_FRONTR, LOW);
  digitalWrite(LX_BACK, LOW);
  delay(50); // waiting for the sensor to change state
  distances.frontL.setAddress(L_FRONTL);
  digitalWrite(LX_LEFT, HIGH); // turning on left sensor
  delay(50); // waiting for the sensor to change state
  distances.left.setAddress(L_LEFT);
  digitalWrite(LX_RIGHT, HIGH); // turning on right sensor
  delay(50); // waiting for the sensor to change state
  distances.right.setAddress(L_RIGHT);
  digitalWrite(LX_FRONTR, HIGH); // turning on front right sensor
  delay(50); // waiting for the sensor to change state
  distances.frontR.setAddress(L_FRONTR);
  digitalWrite(LX_BACK, HIGH); // turning on back sensor
  delay(50); // waiting for the sensor to change state
  distances.back.setAddress(L_BACK);

  temps.left.setAddress(T_LEFT);
  temps.right.setAddress(T_RIGHT);
}

/**
 * Calculates the distance needed to go to the center of the cell.
 * @param distance Distance read from the sensor
 * @return
 */
uint16_t Robot::endDist(uint16_t distance, bool front) {
  if(front){
    distance = distance > CELL_FRONT ? distance - CELL_FRONT : 0;
    return distance - ((distance) % CELL_DIM) + CENTRED;
  }
  distance = distance + CELL_BACK;
  return distance - ((distance) % CELL_DIM) + CENTRED_BACK + CELL_DIM;

}

/**
 * Straightens the robot.
 * Using the two distance sensors tries to place the robot parallel to a wall.
 */
void Robot::straighten(){
  int dif;
  dif=difLaser();
  if(((distances.frontL.read() <= CENTRED<<1)&&(distances.frontR.read() <= CENTRED<<1)) && (dif > 6 ||  dif < -6)){
    mov.rotation(dif>0);
    do{
      dif=difLaser();
    }while( dif > 2|| dif < -2);
    Debug.println("End Straighten");
    mov.stop();
  }
}


/**
 * Centers the robot in the cell if near a wall
 */
void Robot::center(){
  bool right = distances.right.read() < CENTRED2;
  bool left = distances.left.read() < CENTRED2;
  if(left || right){
    rotate(left, 90);
    back(CENTRED-distances.frontL.read());
    rotate(!left,90);
  }
}

float Robot::getInclination() {
  return mov.inclination();
}
 
/**
 * Returns the difference of the front lasers
 */

int Robot::difLaser(){
  int dif=0;
  //for(int i = 0 ; i<3; i++){
  dif=distances.frontL.read()-distances.frontR.read()+LASER_DIF;
  //dif=dif/3;
  Debug.println(String("dif ")+String(dif));
  return dif;
}

/**
 * Reads the battery current voltage.
 * @return The already corrected battery voltage in Volts.
 */
float Robot::getBattery() {
  return ((analogRead(B_PIN) * (3.3f / 4095.0f)) * (B_R1+B_R2))/B_R2;
}
