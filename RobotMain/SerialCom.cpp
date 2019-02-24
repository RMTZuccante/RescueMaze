#include "SerialCom.h"

void SerialCom::waitForSerial() {
  while(!Serial.available());
}

void SerialCom::writeFloat(float f) {
  byte * b = (byte *) &f;
  Serial.write(b[0]);
  Serial.write(b[1]);
  Serial.write(b[2]);
  Serial.write(b[3]);
}

void SerialCom::writeInt(int i) {
  byte * b = (byte *) &i;
  Serial.write(b[0]);
  Serial.write(b[1]);
}
