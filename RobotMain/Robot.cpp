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

void Robot::calibrate() {
  mov.calibrate();
  temps.calibrate();
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
  float out = temps.left.read();
  out = max(out, isVictimL);
  isVictimL = 0;
  return out - temps.ambientL;
}

float Robot::getTempRight() {
	float out = temps.right.read();
	out = max(out, isVictimR);
	isVictimR = 0;
	return out - temps.ambientR;
}

float Robot::getTempAmb() {
  return temps.getAmbient();
}

String Robot::getTemps() {
  return temps;
}

int Robot::go(){
  uint16_t fl=distances.frontL.read();
  uint16_t fr=distances.frontR.read();
  uint16_t b=distances.back.read();
  if( (abs(fl-fr) < cellFront()) && fl < (CELL_DIM *2))return go(true); //Se il robot è prossimo alla cella utilizza il front
  if(b > MAX_RANGE) return go(true); //Altrimenti Se il back è o rischia di andare fuori range
  if(useBack)return go((fl > fr ? fr : fl) <= b); //Altrimenti Se non è dopo una salita controlla la misura minore
  return go (true);
}

/**
 * Goes forward by 30 cm.
 * Tries to go forward by 30 cm, but stops and goes back if a black cell is detected.
 * @return FALSE if it found a black cell
 */
int Robot::go(bool frontLaser) {
  VL53L0X* dist = &(frontLaser?((distances.frontL.read()<distances.frontR.read()) ? distances.frontL : distances.frontR ) : distances.back);
  uint16_t myLaser = 0;
  for(int i=0; i < LASER_READ_TIMES; i++){
    myLaser += dist->read();
  }
  myLaser /= LASER_READ_TIMES;
  uint16_t end = endDist(myLaser,frontLaser); // calcolo a che distanza devo arrivare
  
  myLaser = dist->read();
  Debug.println(String("End")+String(end));
  Debug.println(String(frontLaser?"using front laser":"using back laser"));
  bool charged = false;
  int res = 0;
  int i = 0;
  int salita = 0;
  int weight = 0;
  uint16_t before = myLaser;
  if(color.isBlack()) res=BLACK; // controllo il colore
  uint16_t start = myLaser;
  uint16_t front = 0;
  mov.go(); // inizio a muovermi
  mov.setSpeed(SPEED);
  Debug.println(String("start go "));
  Debug.println(String("First read: ")+myLaser);
  float incl = mov.getRoll();
  while (((front > MAX_RANGE) && (incl > MIN_CHANGE_INCL) ) || ((frontLaser) ? (myLaser > end) : (myLaser < end)) && res!=BLACK) {
    if(!frontLaser){
      front = distances.frontL.read();
    }
    // ogni 7 iterazioni controlla l'ostacolo
    if (i == 7 && (!charged) ) {
      uint16_t now = dist->read();
      if (((before > now) ? before - now : now - before) < MIN_CHANGE_OBS && now < MAX_RANGE) {
        charge();
        charged = true;
        if(res != RISE)res = OBSTACLE;
      }
      before = now;
      i = 0;
    }
    if(res == OBSTACLE) {
      ++weight;
      mov.straighten();
    }
    i++;
    myLaser = dist->read();
    Debug.println(String("Laser read: ")+myLaser);
    incl = mov.getRoll();
    Debug.println(String("Incl : ")+incl);
    // controllo salita
    if (abs(incl) > RISEINCL) {
      ++salita;
      weight = 0;
      if(res != RISE)res = OBSTACLE;
    }
    else salita = 0;

    // se rilevata eseguo salita
    if(salita >= 5){
      Debug.println("Rising");
      useBack = false;
      res = RISE;
      weight = 0;
      for(int i = 0; i < 3; ){
        mov.idle();
        mov.delayr(100);
        incl = mov.getRoll();
        Debug.println(String("Incl: ")+incl);
        if(abs(incl) < MIN_CHANGE_INCL) ++i;
        else i = 0;
      }
      frontLaser = ( (!useBack) || distances.frontL.read() < MAX_RANGE);
      dist = &(frontLaser ? ((distances.frontL.read() < distances.frontR.read()) ? distances.frontL : distances.frontR ) : distances.back);
      end = endDist(dist->read()+CELL_DIM,frontLaser);
      Debug.println(String(frontLaser?"using front laser":"using back laser"));
      Debug.println(String("New End")+String(end));
    }

    // controllo colore
    if(color.isBlack()) res=BLACK;

    // controllo temperature
    if(abs(start-myLaser) > centeredB()){
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
  else if(res != RISE)mov.endGo();
  mov.stop();

  Debug.println(String("stop"));
  straighten();
  //if(res != OBSTACLE) center();
  Debug.println(String("RES: ")+res);
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
  useBack = true;
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
    return distance - ((distance) % CELL_DIM) + centeredF();
  }
  return distance - ((distance) % CELL_DIM) + centeredB() + CELL_DIM;
}

/**
 * Straightens the robot.
 * Using the two distance sensors tries to place the robot parallel to a wall.
 */
void Robot::straighten(){
  int dif=0;
  for(int i = 0; i < LASER_READ_TIMES ; i++){
    dif+=difLaser();
  }
  dif/=LASER_READ_TIMES;
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
  back(100);
  mov.go(false);
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
int Robot::centeredF(){
  return (CELL_DIM- ROBOT_DIM)*FRONT_P;
}
int Robot::centeredB(){
  return (CELL_DIM- ROBOT_DIM)*BACK_P;
}

 int Robot::cellFront(){
  return CELL_DIM - ROBOT_DIM;
 }
