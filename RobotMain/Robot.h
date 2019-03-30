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
    return frontL.check() && frontR.check() && left.check() && right.check() && back.check();
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
};

struct Temps {
  Temperature left;
  Temperature right;

  bool check() {
    return left.check() && right.check();
  }
};

class Robot {
  public:
    void setup();
    void begin();
    bool check();
    bool checkBattery();

    uint16_t getDistance(int sensor);
    ColorData getColor();
    void setBlackThreshold(uint8_t black_threshold);
    float getTempLeft();
    float getTempRight();

    void straighten();
    int go();
    int go(bool frontLaser);
    void back(uint16_t length);
    void back();
    void rotate(bool dir, float angle);
    void rotate(bool dir);
    void victim(int n);
    void climb(int k);
    void laserTest();
    void tempTest();
    void setLED(bool red, bool green, bool blue);

    void delayr(unsigned int t);
  private:
    Color color;
    Distances distances;
    Temps temps;

    Moviment mov = Moviment(SPEED, &temps.left, &temps.right);
    RGB led = RGB(RED, GREEN, BLUE);
    Cagamattoni caga = Cagamattoni(10);
    int difLaser();
    float getBattery();
    void setAddresses();
    uint16_t endDist(uint16_t distance, bool front);
    float isVictimL;
    float isVictimR;
};

#endif
