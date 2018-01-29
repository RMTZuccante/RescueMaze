#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

#define WAIT 5

class RGB {
  public:
    RGB(byte pinr, byte ping, byte pinb);
    void set(byte red, byte green, byte blue);
    void crossFade();
	void begin();
  private:
    void refresh();
    int calculateStep(int prevValue, int endValue);
    int calculateVal(int step, int val, int i);
    void crossFade(byte color[3]);
    //Variables
    byte pin[3];
    byte actual[3] = {0, 0, 0};
    byte previous[3] = {0, 0, 0};

    byte black[3]  = { 0, 0, 0 };
    byte white[3]  = { 100, 100, 100 };
    byte red[3]    = { 100, 0, 0 };
    byte green[3]  = { 0, 100, 0 };
    byte blue[3]   = { 0, 0, 100 };
    byte yellow[3] = { 40, 95, 0 };
    byte dimWhite[3] = { 30, 30, 30 };
};

RGB::RGB(byte pinr, byte ping, byte pinb) {
  pin[0] = pinr;
  pin[1] = ping;
  pin[2] = pinb;
}
void RGB::begin(){
	for (int i = 0; i < 3; i++) pinMode(pin[i], OUTPUT);
}
void RGB::set(byte red, byte green, byte blue) {
  pwmWrite(pin[0], red);
  pwmWrite(pin[1], green);
  pwmWrite(pin[2], blue);
}

void RGB::refresh() {
  set(actual[0], actual[1], actual[2]);
}

void RGB::crossFade() {
  crossFade(red);
  crossFade(green);
  crossFade(blue);
  crossFade(yellow);
}

void RGB::crossFade(byte color[3]) {
  for (int i = 0; i < 3; i++) color[i]=(color[i] * 255) / 100;
  int step[3];
  for (int i = 0; i < 3; i++) step[i] = calculateStep(previous[i], color[i]);
  for (int i = 0; i <= 1020; i++) {
    for (int j = 0; j < 3; j++) actual[j] = calculateVal(step[j], actual[j], i);
    refresh();
    delay(WAIT);
  }
  for (int i = 0; i < 3; i++) previous[i]=actual[i];
}

int RGB::calculateVal(int step, int val, int i) {
  if (step && i % step == 0) {
    if (step > 0) val += 1;
    else if (step < 0) val -= 1;
  }
  if (val > 255) val = 255;
  else if (val < 0) val = 0;
  return val;
}

int RGB::calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) step = 1020 / step;
  return step;
}

#endif
