#include "SerialCom.h"

void SerialCom::begin() {
  Serial.begin(115200);
}

bool SerialCom::check() {
  boolean connected = false;
  while (!connected) {
    waitForSerial();
    if (getCommand() == Commands::HANDSHAKE) {
      Serial.write(Serial.read() * 2);
      connected = true;
    }
    else while (Serial.available()) Serial.read();
    return connected;
  }
}

void SerialCom::waitForSerial() {
  while(!Serial.available());
}

void SerialCom::writeString(String s) {
    Serial.write(STX);
    Serial.print(s);
    Serial.write(ETX);
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

void SerialCom::writeRes(byte res) {
  Serial.write(RES | res);
}

void SerialCom::writeReady() {
  Serial.write(READY);
}

Commands SerialCom::getCommand() {
  return static_cast<Commands>(Serial.read());
}
