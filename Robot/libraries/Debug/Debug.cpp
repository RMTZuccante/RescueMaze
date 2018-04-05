#include "Debug.h"

/**
 * Add the given string to the same line buffer.
 * This function won't send until println is called.
 * @param st String to print.
 * @param level Debug level.
 */
void SerialDebug::print(String st, int level) {
  buffer+=st;
}

/**
 * Add the given string to the line buffer, and send it, with a level of DEBUG.
 * @param st String to print.
 * @param level Debug level.
 */
void SerialDebug::println(String st, int level) {
  print(st);
  Serial.println(getLevel(level) + buffer);
  buffer = "";
}

/**
 * Add the given string to the same line buffer, with a level of DEBUG.
 * @param st String to print.
 */
void SerialDebug::print(String st) {
  print(st, LVL_DEBUG);
}

/**
 * Add the given string to the line buffer, and send it, with a level of DEBUG.
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
  if(level==LVL_DEBUG) delay(t);
}

/**
 * Sets the level to be used for the debug.
 * @param lvl Debug level.
 */
void SerialDebug::setLevel(int lvl) {
  level = lvl;
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

void SerialDebug::wait() {
  if(level==LVL_DEBUG) {
    Serial.println("Waiting...");
    String m;
    do {
      m = readLine();
    } while (m != "ok");
  }
}

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
