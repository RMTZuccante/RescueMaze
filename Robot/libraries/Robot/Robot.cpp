#include "Robot.h";

void Robot::setup() {
  setAddresses();
}

void Robot::begin() {
  for (int i = 0 ; i < 3 ; i++) laser[i].begin();
  //color.begin();
  mov.begin();
}

void Robot::laserTest(){
  Serial.print(" 0: ");
  Serial.print(laser[0].read());
  Serial.print(" 1: ");
  Serial.print(laser[1].read());
  Serial.print(" 2: ");
  Serial.println(laser[2].read());
  
}

bool Robot::check() {
  bool ok = true;
  for (int i = 0 ; i < 3 ; i++) ok &= laser[i].check();
  //return ok && color.check() && mov.check();
  return ok && mov.check();
}

void Robot::update() {
  for (int i = 0; i < 3; i++) data.dist[i] = laser[i].read();
  //data.color = color.read();
  data.tempL = tempL.read();
  data.tempR = tempR.read();
  data.pitch = mov.getPitch();
}

void Robot::go() {
  laser[0].start();
  delay(50);
  uint16_t end = endDist(laser[0].read());
  Serial.print("laser : ");
  Serial.println(laser[0].read());
  Serial.print("end : ");
  Serial.println(end);
 
  mov.go();
  //while ( (laser[0].read() > end) && (color.read() == 2) );
  while (laser[0].read() > end){
    Serial.print(laser[0].read());
    Serial.print(" ");
    Serial.println(end);
    mov.setSpeed(((laser[0].read()-end)*50)+SPEED);
//    mov.straight();
  }
  mov.stop();
  laser[0].stop();
}

void Robot::back() {
  laser[0].start();
  mov.go(true);
  uint16_t end = endDist(laser[0].read()) + 300;
  while (laser[0].read() < end);
  mov.stop();
  laser[0].stop();
}

void Robot::rotate(bool dir) {
  mov.rotate(dir);
}

void Robot::victim() {
  //cagamattoni.caga();
  led.set(0, 0, 0);
}

void Robot::setAddresses() {
  pinMode(LX_LEFT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONT, OUTPUT_OPEN_DRAIN);
  digitalWrite(LX_FRONT, LOW);
  digitalWrite(LX_LEFT, LOW);
  laser[1].setAddress(L_RIGHT);
  digitalWrite(LX_FRONT, HIGH);
  delay(10);
  laser[0].setAddress(L_FRONT);
  digitalWrite(LX_LEFT, HIGH);
  delay(10);
  laser[2].setAddress(L_LEFT);
}

uint16_t Robot::endDist(uint16_t distance) {
  distance = distance > CELL ? distance-CELL : 0;;
  return distance - ((distance) % 300) + CENTRED;
}
