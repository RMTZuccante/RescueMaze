#include "Cagamattoni.h"

/**
 * Sets the cagamattoni's number of available caps.
 * @param ntappi Number of available caps.
 */
Cagamattoni::Cagamattoni(int ntappi, VL53L0X* laser) {
  kits = ntappi;
  this->laser = laser;
}

/**
 * Makes the cagamattoni ready to be used.
 */
void Cagamattoni::begin() {
  pinMode(CAGA1, OUTPUT);
  pinMode(CAGA2, OUTPUT);
  digitalWrite(CAGA1, 0);
  digitalWrite(CAGA2, 0);
}

/**
 * Releases a single cap.
 */
void Cagamattoni::caga() {
  int time;
  bool released = false;
  for(int i = 0; !released && i < 3; i++) {
    time = millis()+CAGADEL; 
    turnOut();
    while(!released && millis()<time) released = laser->read() < CAGADIST;
    stop();
    delay(MINDEL);    
    if (!released) {
      time = millis()+CAGADEL; 
      turnIn();
      while(!released && millis()<time) released = laser->read() < CAGADIST;
      stop();
    }
    else {
      turnIn();
      delay(MINDEL);
      stop();
      delay(MINDEL);
      turnOut();
      delay(MINDEL);
      stop();
      kits--;
    }
  }
  if (!released) kits = 0;
}

/**
 * Check if there are some taps left.
 * @return TRUE if there are 0 caps left.
 */
bool Cagamattoni::isEmpty() {
  return kits < 1;
}

/**
 * Turns the caps towards the inside of the cagamattoni.
 */
void Cagamattoni::turnOut() {
  digitalWrite(CAGA1, 0);
  digitalWrite(CAGA2, 1);
}

/**
 * Turns the caps towards the inside of the cagamattoni.
 */
void Cagamattoni::turnIn() {
  digitalWrite(CAGA1, 1);
  digitalWrite(CAGA2, 0);
}

/**
 * Stops the cagamattoni rotation.
 */
void Cagamattoni::stop() {
  digitalWrite(CAGA1, 0);
  digitalWrite(CAGA2, 0);
}
