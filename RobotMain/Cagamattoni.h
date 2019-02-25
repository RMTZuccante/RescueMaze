#ifndef CAGAMATTONI_H
#define CAGAMATTONI_H

#include <Arduino.h>
#include "VL53L0X.h"
#include "definitions.h"

class Cagamattoni {
  public :
    Cagamattoni(int ntappi);
    void caga();
    bool isEmpty();
    void stop();
    void begin();
  private :
    byte kits;
};

#endif
