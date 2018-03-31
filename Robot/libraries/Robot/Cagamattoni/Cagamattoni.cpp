#include "Cagamattoni.h"

/**
 * Sets the cagamattoni's number of available caps.
 * @param ntappi Number of available caps.
 */
Cagamattoni::Cagamattoni(int ntappi) {
  kits = ntappi;
}

/**
 * Makes the cagamattoni ready to be used.
 */
void Cagamattoni::begin() {
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);

  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 0);
}

/**
 * Releases a single cap.
 */
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

/**
 * Check if there are some taps left.
 * @return TRUE if there are 0 caps left..
 */
bool Cagamattoni::isEmpty() {
  return kits < 1;
}

/**
 * Turns the caps towards the inside of the cagamattoni.
 */
void Cagamattoni::turnOut() {
  digitalWrite(PIN2, 0);
  digitalWrite(PIN1, 1);
}

/**
 * Turns the caps towards the inside of the cagamattoni.
 */
void Cagamattoni::turnIn() {
  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 1);
}

/**
 * Stops the cagamattoni rotation.
 */
void Cagamattoni::stop() {
  digitalWrite(PIN1, 0);
  digitalWrite(PIN2, 0);
}
