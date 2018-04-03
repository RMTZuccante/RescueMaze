#include "Robot.h";
#include <math.h>
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
  for (int i = 0 ; i < 4 ; i++) laser[i].begin();
  //color.begin();
  mov.begin();
}

/**
 * Prints one reading from each distance sensor.
 */
void Robot::laserTest() {
  Serial.print(" 0: ");
  Serial.print(laser[0].read());
  Serial.print(" 1: ");
  Serial.print(laser[1].read());
  Serial.print(" 2: ");
  Serial.print(laser[2].read());
  Serial.print(" 3: ");
  Serial.println(laser[3].read());
}

void Robot::climb() {
  mov.stop();
}

/**
 * Check if every sensor is connected and working.
 * @return TRUE if everything is ok
 */
bool Robot::check() {
  bool ok = true;
  for (int i = 0 ; i < 4 ; i++) ok &= laser[i].check();
  //return ok && color.check() && mov.check();
  return ok && mov.check();
}

/**
 * Check if the battery is charged enough for the robot to work
 * @return TRUE if the battery voltage is higher than 11 volts
 */
bool Robot::checkBattery() {
  return getBattery()>11.f;
}

/**
 * Updates all data inside the data package.
 */
void Robot::update() {
  for (int i = 0; i < 3; i++) data.dist[i] = laser[i].read();
  data.color = 0; //color.read();
  float tempAmb = (tempL.readAmb() + tempR.readAmb()) / 2;
  data.tempL = tempL.read() - tempAmb;
  data.tempR = tempR.read() - tempAmb;
  data.pitch = mov.getPitch();
}

/**
 * Goes forward by 30 cm.
 * Tries to go forward by 30 cm, but stops and goes back if a black cell is detected.
 * @return FALSE if it found a black cell
 */
bool Robot::go() {
  //  Serial.println("go");
  laser[0].start();
  delay(50);
  laser[0].read();
  uint16_t end = endDist(laser[0].read());
  //Serial.print(end);
  //  uint16_t front=laser[0].read();
  //  uint16_t end = (front > 300) ? front-300 : CENTRED;
  //    Serial.print("laser : ");
  //    Serial.println(laser[0].read());
  //    Serial.print("end : ");
  //    Serial.println(end);
  int i = 0;
  uint16_t front = laser[0].read();
  bool black = false;//(color.read() == 2);
  mov.go();
  //while ( (laser[0].read() > end) && !black );
  while (laser[0].read() > end) {
    //int time=millis();
    if (i == 100) {
      //Serial.println("if");
      uint16_t now = laser[0].read();
      if ( ( (front > now) ? front - now : now - front ) < 5) {
        mov.setSpeed(65355);
        delay(200);
        //        Serial.println("bump");
        //        delay(1000);
      }
      front = now;
      i = 0;
      //black = (color.read() == 2);
    }
    //        Serial.print(laser[0].read());
    //        Serial.print(" ");
    //        Serial.println(end);
    //    Serial.println(i);
    i++;
    uint16_t front = laser[0].read();
    //Serial.println(millis()-time);
    mov.setSpeed(((laser[0].read() - end) * 20) + SPEED);
    mov.straight();
  }

  mov.go(false);
  mov.stop();
  
  //delay(1000);
  //Serial.println("engGo");  
  mov.endGo();
  //Serial.println("engGo finished");
  mov.stop();
  laser[0].stop();
  //Serial.println("finished");
  return !black;
}

/**
 * Goes back to the center of the cell.
 */
void Robot::back() {
  laser[0].start();
  uint16_t end = endDist(laser[0].read()) + 300;
  mov.go(true);
  while (laser[0].read() < end);
  mov.stop();
  laser[0].stop();
}

/**
 * Rotates the robot by 90 degrees.
 * @param dir TRUE to turn right, false to turn left
 */
void Robot::rotate(bool dir) {
  mov.rotate(dir);
}

/**
 * Signals if a victim has been found using led and cagamattoni.
 */
void Robot::victim() {
  //cagamattoni.caga();
  for (int i = 0; i < 5; i++) {
    led.set(HIGH, LOW, LOW);
    delay(100);
    led.set(LOW, LOW, LOW);
    delay(100);
  }
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
  pinMode(LX_LEFT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_RIGHT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONTL, OUTPUT_OPEN_DRAIN);
  digitalWrite(LX_LEFT, LOW);
  digitalWrite(LX_RIGHT, LOW);
  digitalWrite(LX_FRONTL, LOW);
  
  laser[0].setAddress(L_FRONTR);
  
  digitalWrite(LX_FRONTL, HIGH);
  delay(10);
  laser[3].setAddress(L_FRONTL);
  
  digitalWrite(LX_RIGHT, HIGH);
  delay(10);
  laser[1].setAddress(L_RIGHT);
  
  digitalWrite(LX_LEFT, HIGH);
  delay(10);
  laser[2].setAddress(L_LEFT);
}

/**
 * Calculates the distance needed to go to the center of the cell.
 * @param distance Distance read from the sensor
 * @return
 */
uint16_t Robot::endDist(uint16_t distance) {
  distance = distance > CELL ? distance - CELL : 0;;
  return distance - ((distance) % 300) + CENTRED;
}

/**
 * Straightens the robot.
 * Using the two distance sensors tries to place the robot parallel to a wall.
 */
void Robot::straighten(){
  laser[0].start();
  laser[3].start();
  uint16_t laser1=laser[0].read();
  uint16_t laser2=laser[3].read();
  /*
    int dif;
    do{
    laser[0].read();
    laser[3].read();
  	dif=0;
  	for(int i = 0 ; i<3; i++){
      dif=dif+laser[0].read();
      dif=dif-laser[3].read();
    }
    dif=dif/3;
    mov.rotate((dif > 0) , 1);
  }while( dif > 5 || dif < -5);
  */
  //(laser1 > laser2) ? mov.rotate(true,atan2((laser1-laser2),LASER_DIST)) : mov.rotate(false,atan2((laser2-laser1),LASER_DIST));
  
  laser[0].stop();
  laser[3].stop();
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
void Robot::delay(unsigned int t) {
  unsigned int end = millis() + t;
  while (end < millis()) mov.idle();
}
