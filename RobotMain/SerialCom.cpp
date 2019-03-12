#include "SerialCom.h"

void SerialCom::begin() {
  Serial.begin(115200);
}

bool SerialCom::check() {
  boolean connected = false;
  while (!connected) {
    wait();
    if (getCommand() == Commands::HANDSHAKE) {
      Serial.write(Serial.read() * 2);
      connected = true;
    }
    else while (Serial.available()) Serial.read();
    return connected;
  }
}

void SerialCom::wait() {
  while(!Serial.available());
}

void SerialCom::write(String s) {
    Serial.write(STX);
    Serial.print(s);
    Serial.write(ETX);
}

void SerialCom::write(float f) {
  byte * b = (byte *) &f;
  Serial.write(b[0]);
  Serial.write(b[1]);
  Serial.write(b[2]);
  Serial.write(b[3]);
}

void SerialCom::write(int i) {
  byte * b = (byte *) &i;
  Serial.write(b[0]);
  Serial.write(b[1]);
}

void SerialCom::write(Commands c) {
  Serial.write(static_cast<byte>(c));
}

void SerialCom::notifyRes(byte res) {
  Serial.write(RES | res);
}

void SerialCom::notifyReady() {
  Serial.write(READY);
}

byte SerialCom::read() {
  wait();
  return Serial.read();
}

Commands SerialCom::getCommand() {
  return static_cast<Commands>(read());
}
