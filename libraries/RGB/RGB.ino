#include "RGB.h"
RGB led(11,13,12);


void blink() {
  led.begin();
  led.set(0,0,255);
  delay(200);
  led.set(0,0,0);
  delay(200);
  led.set(255,0,0);
  delay(200);
  led.set(0,0,0);
  delay(200);  
}

void setup(){
  
}

void loop() {
  for(int i = 0; i<2; i++) blink(); 
  delay(2000);
}





