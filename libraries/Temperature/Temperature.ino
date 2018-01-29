#include <Arduino.h>

#include "Temperature.h"

Temperature t1 = Temperature(0x5A);
Temperature t2 = Temperature(0x5B);

void setup() {
  Serial.begin(9600);
  t1.begin();
  t2.begin();
}

void loop() {
  Serial.print(t1.readObj());
  Serial.print("\t");
  Serial.println(t2.readObj());
}
