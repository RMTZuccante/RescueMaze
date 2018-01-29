#include "Cagamattoni.h"
Cagamattoni cagamattoni(6);
void setup() {
  cagamattoni.begin();
}

void loop() {
    if (!cagamattoni.isEmpty())
    cagamattoni.caga();
    delay(2000);
}

