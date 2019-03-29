#include "RGB.h"

/**
 * Sets the LED pins.
 * @param pinr Pin of the red LED.
 * @param ping Pin of the green LED.
 * @param pinb Pin of the blue LED.
 */
RGB::RGB(uint8_t pinr, uint8_t ping, uint8_t pinb) {
  pin[0] = pinr;
  pin[1] = ping;
  pin[2] = pinb;
}

/**
 * Sets the LED ready to be set.
 */
void RGB::begin() {
  for (int i = 0; i < 3; i++) pinMode(pin[i], OUTPUT);
}

/**
 * Sets the RGB led.
 * @param red Intensity of the red led
 * @param green Intensity of the green led
 * @param blue Intensity of the blue led
 */
void RGB::set(bool red, bool green, bool blue) {
  digitalWrite(pin[0], !red);
  digitalWrite(pin[1], !green);
  digitalWrite(pin[2], !blue);
}
