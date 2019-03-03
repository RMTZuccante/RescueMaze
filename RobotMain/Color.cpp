#include "Color.h"

/**
 * Starts the sensor and makes it ready to be read.
 */
void Color::begin() {
  apds.begin();
  apds.enableLightSensor(false);
}

/**
 * Check if the sensor is working.
 * @return TRUE if it works.
 */
bool Color::check() {
  return apds.check();
}

/**
 * Reads the cell state read from the sensors.
 * @return 0 if it detects a white cell, 1 for mirror and 2 for black.
 */
uint8_t Color::read() {
  uint16_t ambient_light = apds.readAmbientLight();
  uint16_t red_light = apds.readRedLight();
  uint16_t green_light = apds.readGreenLight();
  uint16_t blue_light = apds.readBlueLight();

  return (green_light > red_light && green_light > blue_light)?0:((ambient_light < 50)?1:0);
}
