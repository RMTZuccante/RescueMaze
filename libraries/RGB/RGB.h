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
    byte white[3]  = { 30000, 30000, 30000 };
    byte red[3]    = { 30000, 0, 0 };
    byte green[3]  = { 0, 30000, 0 };
    byte blue[3]   = { 0, 0, 30000 };
    byte yellow[3] = { 10000, 30000, 0 };
    byte dimWhite[3] = { 10000, 10000, 10000 };
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


#endif