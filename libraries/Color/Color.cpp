#include "Color.h"

void Color::begin(){
  Wire.begin();
  Wire.beginTransmission(COLORADDRESS);
  Wire.write(10+MERROR);
  Wire.endTransmission();
}

byte Color::read(){ 
  Wire.requestFrom(COLORADDRESS,1);
  return Wire.read();
}