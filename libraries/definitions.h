#ifndef definitions_h
#define definitions_h

#include <Wire.h>
extern TwoWire Wire2(2);

//Temperature adresses
#define T_LEFT 0x5A
#define T_RIGHT 0x5B

//Laser addresses
#define L_LEFT 0x29
#define L_FRONT 0x2A
#define L_RIGHT 0x2B
//Laser pin
#define LX_LEFT PA15
#define LX_FRONT PA10

//RGB
#define RED 4
#define GREEN 5
#define BLUE 6

//Directions
#define FRONT 0
#define RIGHT 1
#define LEFT 2
#define BACK 3

//Misurements
#define CENTRED 60 //distanza del robot rispetto al muro per essere centrato

#endif
