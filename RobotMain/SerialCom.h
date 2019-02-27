#ifndef SERIALCOM_H_
#define SERIALCOM_H_

#include <Arduino.h>

enum class Commands {
  EMPTY, HANDSHAKE, ROTATE, GO, GETDISTANCES, GETCOLOR, GETTEMPS, VICTIM, SETDEBUG
};

#define RES   0b10000000
#define STX   0b00000010
#define ETX   0b00000011
#define READY 0b00001000

class SerialCom {
public:
  void begin();
  boolean check();
  void wait();
  void write(String s);
	void write(float f);
	void write(int i);
  void notifyRes(byte res);
  void notifyReady();
  byte read();
  Commands getCommand();
};

extern SerialCom Com;

#endif
