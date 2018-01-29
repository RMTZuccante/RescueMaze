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

Cagamattoni::Cagamattoni(int ntappi) {
  kits = ntappi;
}
void Cagamattoni::begin(){
    pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);

  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 0);
}
void Cagamattoni::caga() {
  digitalWrite(PIN2, 1);
  delay(DELAY);
  digitalWrite(PIN2, 0);
  delay(MINDEL);
  digitalWrite(PIN1, 1);
  delay(MINDEL);
  digitalWrite(PIN1, 0);
  delay(MINDEL);
  digitalWrite(PIN2, 1);
  delay(MINDEL);
  digitalWrite(PIN2, 0);
  kits--;
}

bool Cagamattoni::isEmpty() {
  return kits < 1;
}

void Cagamattoni::turnOut() {
  digitalWrite(PIN2, 0);
  digitalWrite(PIN1, 1);
}

void Cagamattoni::turnIn() {
  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 1);
}

void Cagamattoni::stop() {
  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 0);
}

#endif
