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
  cagamattoni.attach(CAGAPIN);
  cagamattoni.write(50);
}

/**
 * Releases a single cap.
 */
void Cagamattoni::caga() {
  cagamattoni.write(120);
  delay(250); // DANGER
  cagamattoni.write(50);
  delay(250); // DANGER
  kits--;
}

/**
 * Check if there are some taps left.
 * @return TRUE if there are 0 caps left.
 */
bool Cagamattoni::isEmpty() {
  return kits < 1;
}
