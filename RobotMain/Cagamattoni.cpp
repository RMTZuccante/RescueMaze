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

}

/**
 * Releases a single cap.
 */
void Cagamattoni::caga() {
  //TODO
}

/**
 * Check if there are some taps left.
 * @return TRUE if there are 0 caps left.
 */
bool Cagamattoni::isEmpty() {
  return true;
}
