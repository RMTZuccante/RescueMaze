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
 * @return a ColorData struct containing values read.
 */
ColorData Color::read() {
  ColorData data;

  data.ambient = apds.readAmbientLight();
  data.red = apds.readRedLight();
  data.green = apds.readGreenLight();
  data.blue = apds.readBlueLight();

  return data;
}

bool Color::isBlack() {
  return apds.readAmbientLight() < black_threshold;
}
