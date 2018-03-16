#ifndef Robot_h
#define Robot_h

#include <Arduino.h>

#include "I2C.h"
#include "Moviment.h"
#include "Color.h"
#include "Temperature.h"
#include "RGB.h"
#include "VL53L0X.h"

#include "definitions.h"

struct RobotData {
  uint16_t dist[3];
  uint8_t color;
  float tempL;
  float tempR;
  float pitch;
};

class Robot {
  public:
    void climb();
    void setup();
    void begin();
    bool check();
    void update();

    bool go();
    void back();
    void rotate(bool dir);
    void victim();
    void laserTest();
    RobotData data;
  private:
    Moviment mov = Moviment(SPEED);
    Color color = Color(MERROR, MCOLOR);
    Temperature tempL = Temperature(T_LEFT);
    Temperature tempR = Temperature(T_RIGHT);
    RGB led = RGB(RED, GREEN, BLUE);
    VL53L0X laser[3] = {VL53L0X(), VL53L0X(), VL53L0X()};

    void setAddresses();
    uint16_t endDist(uint16_t distance);
};

#endif
