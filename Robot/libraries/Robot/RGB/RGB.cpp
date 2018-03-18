#include "RGB.h"

RGB::RGB(uint8_t pinr, uint8_t ping, uint8_t pinb) {
  pin[0] = pinr;
  pin[1] = ping;
  pin[2] = pinb;
}
void RGB::begin() {
  for (int i = 0; i < 3; i++) pinMode(pin[i], OUTPUT);
}
void RGB::set(bool red, bool green, bool blue) {
  digitalWrite(pin[0], !red);
  digitalWrite(pin[1], !green);
  digitalWrite(pin[2], !blue);
}
