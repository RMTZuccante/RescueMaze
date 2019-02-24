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

class Robot {
  public:
    void setup();
    void begin();
    bool check();
    bool checkBattery();

    uint16_t* getDistances();
    uint8_t getColor();
    float getTempLeft();
    float getTempRight();

    void straighten();
    int go();
    int go(bool frontLaser);
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
    Temperature tempL = Temperature(T_LEFT);
    Temperature tempR = Temperature(T_RIGHT);
    Moviment mov = Moviment(SPEED, &tempL, &tempR);
    RGB led = RGB(RED, GREEN, BLUE);
    VL53L0X laser[5] = {VL53L0X(), VL53L0X(), VL53L0X(), VL53L0X(), VL53L0X()};
    Cagamattoni caga = Cagamattoni(5,&laser[4]);

    float tempk;
    int difLaser();
    float getBattery();
    void setAddresses();
    uint16_t endDist(uint16_t distance, bool front);
    bool isVictimL;
    bool isVictimR;
};

#endif
