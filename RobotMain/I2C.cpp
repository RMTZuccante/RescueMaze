#include "I2C.h"

void I2C::scan(WireBase *i2c) {
  byte error, address;
  int nDevices;
  Debug.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    i2c->beginTransmission(address);
    error = i2c->endTransmission();
    if (error == 0) {
      Debug.print("I2C device found at address 0x");
      if (address < 16) Debug.print("0");
      Debug.println(String(address, HEX));
      nDevices++;
    }
    else if (error == 4) {
      Debug.print("Unknown error at address 0x");
      if (address < 16) Debug.print("0");
      Debug.println(String(address, HEX));
    }
  }
  if (nDevices == 0) Debug.println("No I2C devices found");
  else Debug.println("done");
}
