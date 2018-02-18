#include <Wire.h>
#include "VL53L0X.h"

//Laser addresses
#define L_LEFT 0x29
#define L_FRONT 0x2A
#define L_RIGHT 0x2B
//Laser pin
#define LX_LEFT PA15
#define LX_FRONT PA10

VL53L0X laser[3] = {VL53L0X(), VL53L0X(), VL53L0X()};

void setAddresses() {
  pinMode(LX_LEFT, OUTPUT);
  pinMode(LX_FRONT, OUTPUT);
  laser[2].setAddress(L_RIGHT);
  pinMode(LX_FRONT, INPUT);
  delay(10);
  laser[1].setAddress(L_FRONT);
  pinMode(LX_LEFT, INPUT);
  delay(10);
  laser[0].setAddress(L_LEFT);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  for(int i = 0 ; i<3 ; i++)laser[i].begin();
}

void loop() {
  Serial.print(laser[0].read());
  Serial.print(",");
  Serial.print(laser[1].read());
  Serial.print(",");
  Serial.println(laser[2].read());
}
