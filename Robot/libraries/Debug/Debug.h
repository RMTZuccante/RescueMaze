#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define LVL_DEBUG 3
#define LVL_INFO 2
#define LVL_WARN 1
#define LVL_OFF 0

class Debug {
public:
  static void print(String st, int level);
  static void println(String st, int level);
  static void print(String st);
  static void println(String st);

  static void delay(int t);
  static void setLevel(int lvl);
private:
  static int level;
  static String buffer;
  Debug() {}
  static String getLevel(int level);
};

#endif
