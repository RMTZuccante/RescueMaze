#include "Color.h"

/**
 * Sets the color sensor parameters.
 */
Color::Color() {
  ambient_light = 0;
  red_light = 0;
  green_light = 0;
  blue_light = 0;
}

/**
 * Starts the sensor and makes it ready to be read.
 */
void Color::begin() {
  apds.init();
  apds.enableLightSensor(false);
}

/**
 * Check if the sensor is working.
 * @return TRUE if it works.
 */
bool Color::check() {
  I2C_1.beginTransmission(0x39);
  bool ok = !I2C_1.endTransmission();
  if(!ok) Debug.println(String("Color sensor at address ") + String(0x39, HEX) + " not found.", LVL_WARN);
  return ok;
}

/**
 * Reads the cell state read from the sensors.
 * @return 0 if it detects a white cell, 1 for mirror and 2 for black.
 */
uint8_t Color::read() {
  return 0;
  apds.readAmbientLight(ambient_light);
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  apds.readBlueLight(blue_light);
  return (green_light > red_light && green_light > blue_light)?1:((ambient_light < 10)?2:0);
}
