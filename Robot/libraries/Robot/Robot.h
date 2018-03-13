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
    void setup();
    void begin();
    bool check();
    void update();

    void go();
    void back();
    void rotate(bool dir);
    void victim();
    
    RobotData data;
  private:
    Moviment mov = Moviment(SPEED);
    Color color = Color(MERROR);
    Temperature tempL = Temperature(T_LEFT);
    Temperature tempR = Temperature(T_RIGHT);
    RGB led = RGB(RED, GREEN, BLUE);
    VL53L0X laser[3] = {VL53L0X(), VL53L0X(), VL53L0X()};
    uint16_t endDist(uint16_t distance);
    void setAddresses();
    
};

#endif
