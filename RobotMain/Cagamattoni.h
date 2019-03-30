#ifndef CAGAMATTONI_H
#define CAGAMATTONI_H

#include <Arduino.h>
#include <Servo.h>
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
    Servo cagamattoni;
};

#endif
