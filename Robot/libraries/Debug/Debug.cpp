#include "Debug.h"

/**
 * Add the given string to the same line buffer.
 * This function won't send until println is called.
 * @param st String to print.
 * @param level Debug level.
 */
void Debug::print(String st, int level) {
  buffer+=st;
}

/**
 * Add the given string to the line buffer, and send it, with a level of DEBUG.
 * @param st String to print.
 * @param level Debug level.
 */
void Debug::println(String st, int level) {
  print(st);
  Serial.println(getLevel(level) + buffer);
}

/**
 * Add the given string to the same line buffer, with a level of DEBUG.
 * @param st String to print.
 */
void Debug::print(String st) {
  print(st, LVL_DEBUG);
}

/**
 * Add the given string to the line buffer, and send it, with a level of DEBUG.
 * @param st String to print.
 */
void Debug::println(String st) {
  println(st, LVL_DEBUG);
}

/**
 * Delay used only in debug. This should be used to avoid errors while running in competition.
 * @param t Time to wait in microseconds.
 */
void Debug::delay(int t) {
  if(level==LVL_DEBUG) delay(t);
}

/**
 * Sets the level to be used for the debug.
 * @param lvl Debug level.
 */
void Debug::setLevel(int lvl) {
  level = lvl;
}

/**
 * Get the string corresponding the actual debug level.
 * @param level Debug level.
 * @return The string representing the level.
 */
String Debug::getLevel(int level) {
  switch (level) {
    case LVL_DEBUG: return "debug ";
    case LVL_INFO: return "information ";
    case LVL_WARN: return "warning ";
    default: return "";
  }
}
