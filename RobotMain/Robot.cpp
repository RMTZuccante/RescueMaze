#include "Robot.h"

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
  return distances.check() && temps.check() && color.check() && mov.check(); // check order is vital
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
  uint16_t fl=distances.frontL.read();
  uint16_t fr=distances.frontR.read();
  uint16_t b=distances.back.read();
  return go(b > MAX_RANGE || ((fl > fr ? fr : fl) <= b) && (abs(fl-fr) < cellFront()));
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
  int weight = 0;
  uint16_t front = dist->read();
  uint16_t before = front;
  if(color.isBlack()) res=BLACK; // controllo il colore
  uint16_t start = front;
  mov.go(); // inizio a muovermi
  mov.setSpeed(SPEED);
  Debug.println(String("start go "));
  Debug.println(String("First read: ")+front);
  while (((frontLaser) ? (front > end) : (front < end)) && res!=BLACK) {
    // ogni 20 iterazioni controllo l'ostacolo
    if (i == 20) {
      uint16_t now = dist->read();
      if (((before > now) ? before - now : now - before) < 3 && now < MAX_RANGE) {
        charge();
        if(res != RISE)res = OBSTACLE;
      }
      before = now;
      i = 0;
    }
    if(res == OBSTACLE) ++weight;
    i++;
    front = dist->read();
    Debug.println(String("Laser read: ")+front);
    // controllo salita
    float incl = mov.getRoll();
    if (abs(incl) > RISEINCL) {
      ++salita;
      weight = 0;
      if(res != RISE)res = OBSTACLE;
    }
    else salita = 0;

    // se rilevata eseguo salita
    if(salita >= 10){
      Debug.println("salita");
      res = RISE;
      weight = 0;
      for(int i = 0; i < 3; ){
        mov.idle();
        mov.delayr(100);
        incl = mov.getRoll();
        if(abs(incl) < RISEINCL-5) ++i;
        else i = 0;
      }
      bool front = (distances.frontL.read()<2000);
      dist = &(front?((distances.frontL.read()<distances.frontR.read()) ? distances.frontL : distances.frontR ) : distances.back);
      end = endDist(dist->read(),front);
    }

    // controllo colore
    if(color.isBlack()) res=BLACK;

    // controllo temperature
    if(abs(start-front) > centered()){
      isVictimL = max(isVictimL, temps.left.read());
      isVictimR = max(isVictimR, temps.right.read());
    }
    mov.idle();
  }
  mov.setSpeed(SPEED);
  Debug.println(String("isVictimL: ")+isVictimL);
  Debug.println(String("isVictimR: ")+isVictimR);
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
  //if(res != OBSTACLE) center();
  return res == OBSTACLE ? weight/5+OBSTACLE : res;
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
  mov.rotate(dir, angle, type);
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
 * Calculates the distance needed to go to the center of the cell.
 * @param distance Distance read from the sensor
 * @return
 */
uint16_t Robot::endDist(uint16_t distance, bool front) {
  if(front){
    distance = distance > cellFront() ? distance - cellFront() : 0;
    return distance - ((distance) % CELL_DIM) + centered();
  }
  return distance - ((distance) % CELL_DIM) + centered()*2/3 + CELL_DIM;

}

/**
 * Straightens the robot.
 * Using the two distance sensors tries to place the robot parallel to a wall.
 */
void Robot::straighten(){
  int dif=0;
  for(int i = 0; i < STR_READ_TIMES ; i++){
    dif+=difLaser();
  }
  dif/=STR_READ_TIMES;
  if(((distances.frontL.read() <= NEAR_WALL)&&(distances.frontR.read() <= NEAR_WALL)) && abs(dif) > MIN_ER && abs(dif) < MAX_ER){
    mov.rotation(dif>0);
    do{
      dif=difLaser();
    }while(abs(dif)>STR_GOAL);
    Debug.println("End Straighten");
    mov.stop();
  }
}


/**
 * Centers the robot in the cell if near a wall
 */
void Robot::center(){
  Debug.println("Center");
  bool left =distances.left.read() < distances.right.read();
  VL53L0X *laser= &(left ? distances.left : distances.right);
  float b = distances.back.read();
  if(laser->read() < SIDE_SPACE && b < LONG_RANGE){
    rotate(left, 30);
    mov.go(true);
    while(laser->read() < SIDE_SPACE);
    rotate(!left,30);
    mov.go(false);
    while(distances.back.read() < b);
    mov.stop();
  }
}

/**
 * Makes the robot charge on the obstacle 
 */
void Robot::charge(){
  mov.setSpeed(MAXSPEED);
  back(50);
  mov.go();
}
 
/**
 * @return The inclination of the robot
 */
float Robot::getInclination() {
  return mov.inclination();
}

/**
 * Returns the difference of the front lasers
 */
int Robot::difLaser(){
  int dif=0;
  dif=distances.frontL.read()-distances.frontR.read()+LASER_DIF;
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

/**
 * Calculates the distance the robot have to hold to be centered
 */
int Robot::centered(){
  return (CELL_DIM- ROBOT_DIM)/2;
}

 int Robot::cellFront(){
  return CELL_DIM - ROBOT_DIM;
 }
