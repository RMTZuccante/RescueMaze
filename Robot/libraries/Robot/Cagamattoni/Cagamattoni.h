#ifndef CAGAMATTONI_H
#define CAGAMATTONI_H

#include <Arduino.h>

#define PIN1 6
#define PIN2 7

#define DELAY 869.56
#define MINDEL 200

class Cagamattoni {
  public :
    Cagamattoni(int ntappi);
    void caga();
    bool isEmpty();
    void turnOut();
    void turnIn();
    void stop();
    void begin();
  private :
    byte kits;
};

#endif
