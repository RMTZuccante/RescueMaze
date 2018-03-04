#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

#define WAIT 5

class RGB {
  public:
    RGB(uint8_t pinr, uint8_t ping, uint8_t pinb);
    void set(uint8_t red, uint8_t green, uint8_t blue);
    void crossFade();
	void begin();
  private:
    void refresh();
    int calculateStep(int prevValue, int endValue);
    int calculateVal(int step, int val, int i);
    void crossFade(uint8_t color[3]);
    //Variables
    uint16_t pin[3];
    uint16_t actual[3] = {0, 0, 0};
    uint16_t previous[3] = {0, 0, 0};

    uint16_t black[3]  = { 0, 0, 0 };
    uint16_t white[3]  = { 30000, 30000, 30000 };
    uint16_t red[3]    = { 30000, 0, 0 };
    uint16_t green[3]  = { 0, 30000, 0 };
    uint16_t blue[3]   = { 0, 0, 30000 };
    uint16_t yellow[3] = { 10000, 30000, 0 };
    uint16_t dimWhite[3] = { 10000, 10000, 10000 };
};


#endif
