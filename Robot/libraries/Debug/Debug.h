#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "definitions.h"

class SerialDebug {
public:
  SerialDebug() : ended(true), level(LVL_DEBUG) {}
  
  void print(String st, int level);
  void println(String st, int level);
  void print(String st);
  void println(String st);

  void delay(int t);
  void wait();
  void setLevel(int lvl);
private:
  String getLevel(int level);
  String readLine();
  bool ended;
  int level;
  String buffer;
};

extern SerialDebug Debug;

#endif
