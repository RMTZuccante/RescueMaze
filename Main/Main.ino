#include "definitions.h"
#include "Moviment.h"
#include "Color.h"
#include "Temperature.h"
#include "RGB.h"
#include "VL53L0X.h"
#include "Matrix.h"

//creating objects
Moviment mov(50000);
Color color;
Temperature tempL (T_LEFT);
Temperature tempR (T_RIGHT);
RGB led(RED, GREEN, BLUE);
VL53L0X laser[3] = {VL53L0X(), VL53L0X(), VL53L0X()};
Matrix matrix;

bool info[2];

#define VICTIM info[1]
#define BLACK info[0]

uint16_t dist[3];

void getDist() {
  for (int i = 0; i < 3; i++) dist[i] = laser[i].read();
}//getDist

void go() {
  laser[0].start();
  mov.go();
  uint16_t end = endDist(laser[0].read());
  while ( (laser[0].read() > end) && (color.read() == 2) );
  mov.stop();
  laser[0].stop();
}//go

void back() {
  laser[0].start();
  mov.go(true);
  uint16_t end = endDist(laser[0].read()) + 300;
  while (laser[0].read() < end);
  mov.stop();
  laser[0].stop();
}

uint16_t endDist(uint16_t distance) {
  return distance - (distance % 300) + CENTRED;
}

void setAddresses() {
  pinMode(LX_LEFT, OUTPUT);
  pinMode(LX_FRONT, OUTPUT);
  laser[1].setAddress(L_RIGHT);
  pinMode(LX_FRONT, INPUT);
  delay(10);
  laser[0].setAddress(L_FRONT);
  pinMode(LX_LEFT, INPUT);
  delay(10);
  laser[2].setAddress(L_LEFT);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire2.begin();
  setAddresses();
  color.begin();
  mov.begin();
  for(int i = 0 ; i<3 ; i++) laser[i].begin();
}//setup

void loop() {
	  getDist();
    matrix.check(dist,tempL.read(),tempR.read(),color.read());
    matrix.getInfo(info);
    if(BLACK){
      back();
      matrix.move(false);
    }
    if(VICTIM){
      //cagamattoni.caga();
      led.set(0,0,0);
    }
    switch(matrix.getDir()){
      case 1 :
        mov.rotate(true);
        break;
      case 2 :
        mov.rotate(false);
        break;
    }//switch
    go();
}//loop
