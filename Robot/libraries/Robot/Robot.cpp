#include "Robot.h";

void Robot::setup() {
  setAddresses();
}

void Robot::begin() {  
  for (int i = 0 ; i < 3 ; i++) laser[i].begin();
  //color.begin();
  mov.begin();
}

void Robot::laserTest() {
  Serial.print(" 0: ");
  Serial.print(laser[0].read());
  Serial.print(" 1: ");
  Serial.print(laser[1].read());
  Serial.print(" 2: ");
  Serial.println(laser[2].read());
}

void Robot::climb() {
  mov.stop();
}

bool Robot::check() {
  bool ok = true;
  for (int i = 0 ; i < 3 ; i++) ok &= laser[i].check();
  //return ok && color.check() && mov.check();
  return ok && mov.check();
}

void Robot::update() {
  for (int i = 0; i < 3; i++) data.dist[i] = laser[i].read();
  data.color = 0; //color.read();
  float tempAmb = (tempL.readAmb() + tempR.readAmb()) / 2;
  data.tempL = tempL.read() - tempAmb;
  data.tempR = tempR.read() - tempAmb;
  data.pitch = mov.getPitch();
}

bool Robot::go() {
  //  Serial.println("go");
  laser[0].start();
  delay(50);
  uint16_t end = endDist(laser[0].read());
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
  mov.endGo();
  mov.stop();
  laser[0].stop();
  return !black;
}

void Robot::back() {
  laser[0].start();
  uint16_t end = endDist(laser[0].read()) + 300;
  mov.go(true);
  while (laser[0].read() < end);
  mov.stop();
  laser[0].stop();
}

void Robot::rotate(bool dir) {
  mov.rotate(dir);
}

void Robot::victim() {
  //cagamattoni.caga();
  for (int i = 0; i < 5; i++) {
    led.set(HIGH, LOW, LOW);
    delay(100);
    led.set(LOW, LOW, LOW);
    delay(100);
  }
}

void Robot::setLED(bool red, bool green, bool blue) {
  led.set(red, green, blue);
}

void Robot::setAddresses() {
  pinMode(LX_LEFT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONTL, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONTR, OUTPUT_OPEN_DRAIN);
  digitalWrite(LX_FRONTL, LOW);
  digitalWrite(LX_FRONTR, LOW);
  digitalWrite(LX_LEFT, LOW);
  laser[1].setAddress(L_RIGHT);
  digitalWrite(LX_FRONTR, HIGH);
  delay(10);
  laser[0].setAddress(L_FRONTR);
  digitalWrite(LX_LEFT, HIGH);
  delay(10);
  laser[2].setAddress(L_LEFT);
  digitalWrite(LX_FRONTL, HIGH);
  delay(10);
  laser[3].setAddress(L_FRONTL);
}

uint16_t Robot::endDist(uint16_t distance) {
  distance = distance > CELL ? distance - CELL : 0;;
  return distance - ((distance) % 300) + CENTRED;
}

void Robot::straighten(){
  
  laser[0].start();
  laser[3].start();
  float dif;
  for(int i=0;i<3;i++){
    dif=dif+laser[0].read()-laser[3].read()+LASER_FL;
  }
  dif=dif/3;
  
  while( dif > 5){
    mov.rotate((dif>0) , 1);
  }
  laser[0].stop();
  laser[3].stop();
}

void Robot::delay(unsigned int t) {
  unsigned int end = millis() + t;
  while (end < millis()) mov.idle();
}
