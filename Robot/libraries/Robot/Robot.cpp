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
  for (int i = 0 ; i < 4 ; i++) laser[i].begin();
  color.begin();
  mov.begin();
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
  Debug.println(String(laser[3].read()));
  Debug.println(String(laser[0].read()-laser[3].read()));
  Debug.println(String(laser[1].read()-laser[0].read()));
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

/**
 * Reads all the data from the robot.
 * @return Pointer to struct containing the data.
 */
RobotData* Robot::read() {
  RobotData *data = new RobotData();
  for (int i = 0; i < 3; i++) data->dist[i] = laser[i].read();
  data->color = color.read();
  float tempAmb = (tempL.readAmb() + tempR.readAmb()) / 2;
  data->tempL = tempL.read() - tempAmb;
  data->tempR = tempR.read() - tempAmb;
  data->pitch = mov.getPitch();
  return data;
}

/**
 * Goes forward by 30 cm.
 * Tries to go forward by 30 cm, but stops and goes back if a black cell is detected.
 * @return FALSE if it found a black cell
 */
bool Robot::go() {
  laser[0].start();
  delay(50);
  laser[0].read();
  uint16_t end = endDist(laser[0].read());
  int i = 0;
  uint16_t front = laser[0].read();
  bool black = (color.read() == 2);
  mov.go();
  while (laser[0].read() > end && !black) {
    if (i == 100) {
      uint16_t now = laser[0].read();
      if ( ( (front > now) ? front - now : now - front ) < 5) {
        mov.setSpeed(65355);
        delay(200);
      }
      front = now;
      i = 0;
      black = (color.read() == 2);
    }
    i++;
    uint16_t front = laser[0].read();
    mov.setSpeed(((laser[0].read() - end) * 20) + SPEED);
    mov.straight();
  }
  mov.go(false);
  mov.stop();
  mov.endGo();
  mov.stop();
  laser[0].stop();
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
  mov.rotation(laser[0].read()>laser[3].read());
  int dif;
  do{
   dif=0;
   for(int i = 0 ; i<3; i++){
      dif=dif+laser[0].read();
      dif=dif-laser[3].read();
      dif=dif+LASER_DIF;
    }
    dif=dif/3;
  }while( dif > 2 || dif < -2);
  mov.stop();
  do{
    laser[0].read();
    laser[3].read();
  	dif=0;
  	for(int i = 0 ; i<3; i++){
      dif=dif+laser[0].read();
      dif=dif-laser[3].read();
      dif=dif+LASER_DIF;
    }
    dif=dif/3;
    Debug.println(String(dif));
    mov.rotate((dif > 0) , 1);
  }while( dif > 1 || dif < -1);
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
  while (end > millis()) mov.idle();
}
