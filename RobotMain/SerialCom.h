#ifndef SERIALCOM_H_
#define SERIALCOM_H_

#include <Arduino.h>

class SerialCom {
public:
	static void handler(int data);
	static void waitForSerial();
	static void writeFloat(float f);
	static void writeInt(int i);
};

#endif
