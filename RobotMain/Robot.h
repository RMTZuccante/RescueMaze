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

  bool check() {
    return left.check() && right.check();
  }

  operator String() {
    String out = "temps={";
    out+="left="+String(left.read());
    out+=",right="+String(right.read());
    return out+"}";
  }
};

class Robot {
  public:
    void setup();
    void begin();
    bool check();
    bool checkBattery();

    uint16_t getDistance(int sensor);
    Distances getDistances();
    ColorData getColor();
    void setBlackThreshold(uint8_t black_threshold);
    float getTempLeft();
    float getTempRight();
    Temps getTemps();
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
};

#endif
