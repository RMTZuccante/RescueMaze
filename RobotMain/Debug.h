#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "SerialCom.h"

enum class Levels{
  OFF, WARN, INFO, DEBUG
};

class SerialDebug {
public:
  SerialDebug() : ended(true), level(Levels::DEBUG) {}
  
  void print(String st, Levels level);
  void println(String st, Levels level);
  void print(String st);
  void println(String st);

  void delayd(int t);
  void setLevel(Levels lvl);
private:
  String getLevel(Levels level);
  bool ended;
  Levels level;
};

extern SerialDebug Debug;

#endif
