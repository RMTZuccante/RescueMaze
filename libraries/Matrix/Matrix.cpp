#include "Matrix.h"

Matrix::Matrix(RobotData &data) {
  this->data = data;
}

bool Matrix::check() {
  int c = 0;
  String m;
  do {
    if (c > 10) return 0;
    m = readLine();
    Serial.println(m);
    m = readLine();
    c++;
  } while (m != "ok");
  return 1;
}

void Matrix::debug(String s) {
  Serial.print("debug ");
  Serial.println(s);
}

void Matrix::update() {
  inspect();
  getInfo();
  getDir();
}

void Matrix::move(bool forward) {
  Serial.print("move ");
  Serial.println(forward ? "forth" : "back");
}

void Matrix::backToStart() {
  Serial.println("tostart");
}

bool Matrix::end() {
  Serial.println("end");
  return readLine()[0] - '0';
}

void Matrix::die() {
  Serial.println("stop");
}

void Matrix::inspect() {
  Serial.print("check ");
  Serial.print(data.dist[0]);
  Serial.print(' ');
  Serial.print(data.dist[1]);
  Serial.print(' ');
  Serial.print(data.dist[2]);
  Serial.print(' ');
  Serial.print(data.tempL);
  Serial.print(' ');
  Serial.print(data.tempR);
  Serial.print(' ');
  Serial.println(data.color);
}

void Matrix::getInfo() {
  Serial.println("getinfo");
  String s = readLine();
  black = s[0] - '0';
  victim = s[2]  - '0';
}

int Matrix::getDir() {
  Serial.println("getdir");
  return readLine().toInt();
}

String Matrix::readLine() {
  while (!Serial.available());
  String s;
  char c = Serial.read();
  do {
    s += c;
    while (!Serial.available());
    c = Serial.read();
  } while (c != '\n');
  return s;
}
