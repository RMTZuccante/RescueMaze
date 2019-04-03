#include "SerialCom.h"

void SerialCom::begin() {
  Serial3.begin(115200);
}

bool SerialCom::check() {
  boolean connected = false;
  while (!connected) {
    wait();
    if (getCommand() == Commands::HANDSHAKE) {
      byte n = read();
      Serial3.write(n * 2);
      connected = read() == 'k' + n / 2;
    }
    else while (Serial3.available()) Serial3.read();
  }
  return connected;
}

void SerialCom::wait() {
  while(!Serial3.available());
}

void SerialCom::write(String s) {
    Serial3.write(STX);
    Serial3.print(s);
    Serial3.write(ETX);
}

void SerialCom::write(float f) {
  byte * b = (byte *) &f;
  Serial3.write(b[0]);
  Serial3.write(b[1]);
  Serial3.write(b[2]);
  Serial3.write(b[3]);
}

void SerialCom::write(int i) {
  byte * b = (byte *) &i;
  Serial3.write(b[0]);
  Serial3.write(b[1]);
}

void SerialCom::write(Commands c) {
  Serial3.write(static_cast<byte>(c));
}

void SerialCom::notifyRes(byte res) {
  Serial3.write(RES | res);
}

void SerialCom::notifyReady() {
  Serial3.write(READY);
}

byte SerialCom::read() {
  wait();
  return Serial3.read();
}

Commands SerialCom::getCommand() {
  return static_cast<Commands>(read());
}

SerialCom Com;
