#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define LVL_DEBUG 3
#define LVL_INFO 2
#define LVL_WARN 1
#define LVL_OFF 0

class SerialDebug {
public:
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
  int level;
  String buffer;
};

extern SerialDebug Debug;

#endif
