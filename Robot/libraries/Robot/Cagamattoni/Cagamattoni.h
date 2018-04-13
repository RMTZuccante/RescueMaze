#ifndef CAGAMATTONI_H
#define CAGAMATTONI_H

#include <Arduino.h>
#include "VL53L0X.h"
#include "definitions.h"

class Cagamattoni {
  public :
    Cagamattoni(int ntappi, VL53L0X* laser);
    void caga();
    bool isEmpty();
    void turnOut();
    void turnIn();
    void stop();
    void begin();
  private :
    byte kits;
    VL53L0X* laser;
};

#endif
