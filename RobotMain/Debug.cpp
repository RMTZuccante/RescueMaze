#include "Debug.h"

/**
 * Sends a string to the Serial port with a debug level, only if it is lower than the setted one. 
 * The debug level will be sent only if it's a new line.
 * @param st String to print.
 * @param level Debug level.
 */
void SerialDebug::print(String st, int level) {
	if (level <= this->level) {
		if (ended) Serial.print(getLevel(level));
		Serial.print(st);
		ended = st.endsWith("\n");
	}
}

/**
 * Sends a line to the Serial port with a debug level, only if it is lower than the setted one. 
 * The debug level will be sent only if it's a new line.
 * @param st String to print.
 * @param level Debug level.
 */
void SerialDebug::println(String st, int level) {
  print(st+"\n", level);
}

/**
 * Sends a debug string to the Serial port, only debug is accepted.
 * The debug level will be sent only if it's a new line.
 * @param st String to print.
 */
void SerialDebug::print(String st) {
  print(st, LVL_DEBUG);
}

/**
 * Sends a debug line to the Serial port, only debug is accepted.
 * The debug level will be sent only if it's a new line.
 * @param st String to print.
 */
void SerialDebug::println(String st) {
  println(st, LVL_DEBUG);
}

/**
 * Delay used only in debug. This should be used to avoid errors while running in competition.
 * @param t Time to wait in microseconds.
 */
void SerialDebug::delay(int t) {
  if(level>=LVL_DEBUG) delay(t);
}

/**
 * Sets the level to be used for the debug.
 * @param lvl Debug level.
 */
void SerialDebug::setLevel(int level) {
  this->level = level;
}

/**
 * Get the string corresponding the actual debug level.
 * @param level Debug level.
 * @return The string representing the level.
 */
String SerialDebug::getLevel(int level) {
  switch (level) {
    case LVL_DEBUG: return "debug ";
    case LVL_INFO: return "information ";
    case LVL_WARN: return "warning ";
    default: return "";
  }
}

/**
 * Waits until "ok" is read from the serial port.
 */
void SerialDebug::wait() {
  if(level>=LVL_DEBUG) {
    Serial.println("Waiting...");
    String m;
    do {
      m = readLine();
    } while (m != "ok");
  }
}

/**
 * Reads a single line from the serial port.
 * @return The line read.
 */
String SerialDebug::readLine() {
  while (!Serial.available());
  String s;
  char c = Serial.read();
  do {
    s += c;
    while (!Serial.available());
    c = Serial.read();
  } while (c != '\n');
  return s;
}
