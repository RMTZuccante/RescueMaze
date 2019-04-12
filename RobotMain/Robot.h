#ifndef Robot_h
#define Robot_h

#include <Arduino.h>
#include <math.h>
#include "definitions.h"

#include "I2C.h"
#include "Debug.h"
#include "Moviment.h"
#include "Cagamattoni.h"
#include "Color.h"
#include "Temperature.h"
#include "RGB.h"
#include "VL53L0X.h"

struct Distances {
  VL53L0X frontL;
  VL53L0X frontR;
  VL53L0X left;
  VL53L0X right;
  VL53L0X back;

  bool check() {
    bool ok = true;
  
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
    frontL.setAddress(L_FRONTL);
    ok &= frontL.check();
    digitalWrite(LX_LEFT, HIGH); // turning on left sensor
    delay(50); // waiting for the sensor to change state
    left.setAddress(L_LEFT);
    ok &= left.check();
    digitalWrite(LX_RIGHT, HIGH); // turning on right sensor
    delay(50); // waiting for the sensor to change state
    right.setAddress(L_RIGHT);
    ok &= right.check();
    digitalWrite(LX_FRONTR, HIGH); // turning on front right sensor
    delay(50); // waiting for the sensor to change state
    frontR.setAddress(L_FRONTR);
    ok &= frontR.check();
    digitalWrite(LX_BACK, HIGH); // turning on back sensor
    delay(50); // waiting for the sensor to change state
    back.setAddress(L_BACK);
    ok &= back.check();
    
    return ok;
  }

  void begin() {
    frontL.begin();
    frontR.begin();
    left.begin();
    right.begin();
    back.begin();
  }

  void start() {
    frontL.start();
    frontR.start();
    left.start();
    right.start();
    back.start();
  }

  operator String() {
    String out = "distances={";
    out+="frontL="+String(frontL.read());
    out+=",frontR="+String(frontR.read());
    out+=",left="+String(left.read());
    out+=",right="+String(right.read());
    out+=",back="+String(back.read());
    return out+"}";
  }
};

struct Temps {
  Temperature left;
  Temperature right;
  float ambientL;
  float ambientR;

  void calibrate() {
    ambientL = left.read();
    ambientR = right.read();
  }

  bool check() {
    left.setAddress(T_LEFT);
    right.setAddress(T_RIGHT);
    return left.check() && right.check();
  }

  float getAmbient() {
    return (ambientL + ambientR) / 2.0f;
  }

  operator String() {
    String out = "temps={";
    out+="left="+String(left.read()-ambientL);
    out+=",right="+String(right.read()-ambientR);
    out+=",ambient="+String(getAmbient());
    return out+"}";
  }
};

class Robot {
  public:
    void setup();
    void begin();
    void calibrate();
    bool check();
    bool checkBattery();

    uint16_t getDistance(int sensor);
    Distances getDistances();
    ColorData getColor();
    void setBlackThreshold(uint8_t black_threshold);
    float getTempLeft();
    float getTempRight();
    float getTempAmb();
    String getTemps();
    float getInclination();
    float getBattery();
    void straighten();
    int go();
    int go(bool frontLaser);
    void back(uint16_t length);
    void back();
    void rotate(bool dir, float angle);
    void rotate(bool dir);
    void rotate(float angle);
    void victim(int n);
    void climb(int k);
    void setLED(bool red, bool green, bool blue);
    void center();
    void charge();
    int centeredF();
    int centeredB();
    int cellFront();
    void delayr(unsigned int t);
  private:
    Color color;
    Distances distances;
    Temps temps;

    Moviment mov = Moviment(SPEED, &temps.left, &temps.right, &isVictimL, &isVictimR);
    RGB led = RGB(RED, GREEN, BLUE);
    Cagamattoni caga = Cagamattoni(10);
    int difLaser();
    void setAddresses();
    uint16_t endDist(uint16_t distance, bool front);
    float isVictimL;
    float isVictimR;
    bool useBack = true;
};

#endif
