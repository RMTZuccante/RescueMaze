#include "Debug.h"

void Debug::print(String st, int level) {
  buffer+=st;
}

void Debug::println(String st, int level) {
  print(st);
  Serial.println(getLevel(level) + buffer);
}

void Debug::print(String st) {
  print(st, LVL_DEBUG);
}

void Debug::println(String st) {
  println(st, LVL_DEBUG);
}


void Debug::delay(int t) {
  if(level==LVL_DEBUG) delay(t);
}

void Debug::setLevel(int lvl) {
  level = lvl;
}

String Debug::getLevel(int level) {
  switch (level) {
    case LVL_DEBUG: return "debug ";
    case LVL_INFO: return "information ";
    case LVL_WARN: return "warning ";
    default: return "";
  }
}
