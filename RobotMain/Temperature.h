#ifndef Temperature_h
#define Temperature_h

#include <Arduino.h>
#include "I2C.h"

#define MLX90614_DEFADDR 0x5A
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07

class Temperature  {
  public:
    Temperature(uint8_t address = MLX90614_DEFADDR);
    void setAddress(uint8_t address);
    bool check();
    float read();
    float readAmb();
  private:
    float readTemp(uint8_t reg);
    uint8_t address;
    uint16_t read16(uint8_t reg);
};

#endif
