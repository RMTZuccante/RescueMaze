#include "Robot.h";
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
  for (int i = 0 ; i < 5 ; i++) laser[i].begin();
  for (int i = 0 ; i < 5 ; i++) laser[i].start();
  //tempk = (tempL.read()+tempL.read())/2+TEMP_DIF;
  tempk=TEMP_K;
}

void Robot::tempTest(){
  Debug.print(" tempL: ");
  Debug.print(String(tempL.read()));
  Debug.print(" tempR: ");
  Debug.println(String(tempR.read()));
}


/**
 * Prints one reading from each distance sensor.
 */
void Robot::laserTest() {
  Debug.print(" 0: ");
  Debug.print(String(laser[0].read()));
  Debug.print(" 1: ");
  Debug.print(String(laser[1].read()));
  Debug.print(" 2: ");
  Debug.print(String(laser[2].read()));
  Debug.print(" 3: ");
  Debug.print(String(laser[3].read()));
  Debug.print(" 4: ");
  Debug.println(String(laser[4].read()));
  //Debug.println(String(laser[0].read()-laser[3].read()));
  //Debug.println(String(laser[1].read()-laser[0].read()));
}

void Robot::climb(int k) {
  Debug.println(String("climb"),LVL_INFO);
  mov.climb(k);
}

/**
 * Check if every sensor is connected and working.
 * @return TRUE if everything is ok
 */
bool Robot::check() {
  bool ok = true;
  for (int i = 0 ; i < 5 ; i++) ok &= laser[i].check();
  return ok && color.check() && mov.check();
}

/**
 * Check if the battery is charged enough for the robot to work
 * @return TRUE if the battery voltage is higher than 11 volts
 */
bool Robot::checkBattery() {
  float battery = getBattery();
  bool ok = getBattery()>11.f;
  Debug.println(String("Battery voltage: ")+battery+"v.",LVL_INFO);
  if(!ok) Debug.println("Critical battery level!", LVL_WARN);
  return ok;
}

uint16_t Robot::getDistance(int sensor) {
	return laser[sensor].read();
}

uint8_t Robot::getColor() {
	return color.read();
}

float Robot::getTempLeft() {
	float out = isVictimL ? 10 : (tempL.read());
	isVictimL = false;
	return out;
}

float Robot::getTempRight() {
	float out = isVictimR ? 10 : (tempR.read());
	isVictimR = false;
	return out;
}

int Robot::go(){
  return go(laser[0].read()<1000);
}

/**
 * Goes forward by 30 cm.
 * Tries to go forward by 30 cm, but stops and goes back if a black cell is detected.
 * @return FALSE if it found a black cell
 */
int Robot::go(bool frontLaser) {
  int dist = frontLaser ? ( (laser[0].read()<laser[3].read()) ? 0 : 3 ) : 4;
  uint16_t end = endDist(laser[dist].read(),frontLaser);
  Debug.println(String("End")+String(end));
  int i = 0;
  int salita = 0;
  uint16_t front = laser[dist].read();
  uint16_t before = front;
  int sol = (color.read() == 2);
  int start = front;
  Debug.println(String(sol));
  mov.go();
  
  Debug.println(String("start go "));
  Debug.println(String("First read: ")+front);
  while ( ( (frontLaser) ? (front > end) : (front < end)) && sol!=1) {
    if (i == 100) {
      uint16_t now = laser[dist].read();
      if ( ( (before > now) ? before - now : now - before ) < 5) {
        mov.impennati(65535);
        while( ( (before > now) ? before - now : now - before ) < 5){
          now = laser[dist].read();
        }
      }
      before = now;
      i = 0;
    }
    i++;
    front = laser[dist].read();
    Debug.println(String("Laser read: ")+front);
    mov.setSpeed(((front - end) * 10) + SPEED);
    mov.straight();
    float incl=mov.inclination();
    (abs(incl) > 10) ? salita++ : salita=0;
    if(salita >= 10){
      Debug.println("salita");
      Debug.println("incl");
      sol = (incl > 0) ? 2 : -2;
      delayr(50);
      while(abs(mov.inclination()) > 8){
        int dif=laser[2].read();
        dif=dif-laser[1].read();
        climb(dif);
      }
      end = endDist(laser[dist].read(),front);
    }
    if(color.read() == 2) sol=1;
    
    float tempAmb = (tempL.readAmb() + tempR.readAmb()) / 2;
    if( abs(start-front) > CENTRED){
      Debug.println(String("is Victim?"));
      isVictimL |= (tempL.read()) > tempk;
      isVictimR |= (tempR.read()) > tempk;
    }
  }
  mov.stop();
  if(sol==1){
    Debug.println(String("Call to back"));
    back();
  }
  mov.endGo();
  mov.stop();
  
  Debug.println(String("stop "));
  straighten();
  return sol;
}

/**
 * Goes back to the center of the cell.
 */
void Robot::back() {
  int dist = ( (laser[0].read()<2000) ? ( (laser[0].read()<laser[3].read()) ? 0 : 3 ) : 4);
  Debug.println(String("back"));
  uint16_t end = endDist(laser[dist].read(), dist!=4) + CELL_DIM;
  Debug.println(String("end ") + String(end));
  mov.go(true);
  uint16_t distance = laser[dist].read();
  if (dist!=4)while (laser[dist].read() < end)distance = laser[dist].read();
  else while (laser[dist].read() > end)distance = laser[dist].read();
  mov.stop();
}

void Robot::rotate(bool dir, float angle) {
  switch(mov.rotate(dir, 90)){
    case 1: 
      isVictimL=true;
      break;
    case 2:
      isVictimR=true;
      break;
  }
  straighten();  
}

/**
 * Rotates the robot by 90 degrees.
 * @param dir TRUE to turn right, false to turn left
 */
void Robot::rotate(bool dir) {
	rotate(dir, 90);
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
      delayr(50);
    }
  }
  else {
    for(int i = 0; i < 5; i++) {
      led.set(LOW, LOW, HIGH);
      delayr(500);
      led.set(LOW, LOW, LOW);
      delayr(500);
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
  laser[0].setAddress(L_FRONTL);
  digitalWrite(LX_LEFT, HIGH); // turning on left sensor
  delay(50); // waiting for the sensor to change state
  laser[2].setAddress(L_LEFT);
  digitalWrite(LX_RIGHT, HIGH); // turning on right sensor
  delay(50); // waiting for the sensor to change state
  laser[1].setAddress(L_RIGHT);
  digitalWrite(LX_FRONTR, HIGH); // turning on front right sensor
  delay(50); // waiting for the sensor to change state
  laser[3].setAddress(L_FRONTR);
  digitalWrite(LX_BACK, HIGH); // turning on back sensor
  delay(50); // waiting for the sensor to change state
  laser[4].setAddress(L_BACK);
}

/**
 * Calculates the distance needed to go to the center of the cell.
 * @param distance Distance read from the sensor
 * @return
 */
uint16_t Robot::endDist(uint16_t distance, bool front) {
  if(front){
    distance = distance > CELL ? distance - CELL : 0;
    return distance - ((distance) % CELL_DIM) + ((distance<CELL_DIM) ? CENTRED : CENTRED2);
  }
  distance = distance + CELL;
  return distance - ((distance) % CELL_DIM) + ((distance<CELL_DIM) ? CENTRED : CENTRED2) + CELL_DIM;
  
}

/**
 * Straightens the robot.
 * Using the two distance sensors tries to place the robot parallel to a wall.
 */
void Robot::straighten(){
  int dif;
  dif=difLaser();
  if(((laser[0].read() <= CENTRED<<1)&&(laser[3].read() <= CENTRED<<1)) && (dif > 6 ||  dif < -6)){
    mov.rotation(dif>0);
    do{
      dif=difLaser();
    }while( dif > 2|| dif < -2);
    mov.stop();
    delayr(500);
//    do{
//      dif=difLaser();
//      mov.rotate((dif > 0) , 1);
//    }while( dif > 2|| dif < -2);
  }
}

/**
 * Returns the difference of the front lasers 
 */

int Robot::difLaser(){
  int dif=0;
  //for(int i = 0 ; i<3; i++){
  dif=dif+laser[0].read();
  dif=dif-laser[3].read();
  dif=dif+LASER_DIF;
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


/**
 * Idles for the given time.
 * Instead of doing nothing this function keep updating filters.
 * @param t Time in milliseconds to wait.
 */
void Robot::delayr(unsigned int t) {
  unsigned int end = millis() + t;
  while (end > millis()) mov.idle();
}
