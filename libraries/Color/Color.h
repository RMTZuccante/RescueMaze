#ifndef Color_h
#define Color_h

#define MERROR 5

#include <Wire.h>

#define COLORADDRESS 42

class Color {
public:
  void begin();
  byte read();
};

#endif
