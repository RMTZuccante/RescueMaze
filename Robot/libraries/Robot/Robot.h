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


struct RobotData {
  uint16_t dist[3];
  uint8_t color;
  float tempL;
  float tempR;
  float pitch;
};

class Robot {
  public:
    void setup();
    void begin();
    bool check();
    bool checkBattery();
    
    RobotData* read();

    void straighten();
    int go();
    int go(bool frontLaser);
    void back();
    void rotate(bool dir);
    void victim(int n);
    void climb(int k);
    void laserTest();
    void tempTest();
    void setLED(bool red, bool green, bool blue);

    void delay(unsigned int t);
  private:
    Moviment mov = Moviment(SPEED);
    Color color = Color(MERROR, MCOLOR);
    Temperature tempL = Temperature(T_LEFT);
    Temperature tempR = Temperature(T_RIGHT);
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
