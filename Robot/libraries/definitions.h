#ifndef definitions_h
#define definitions_h

//Settings
#define SPEED 20000
#define MERROR 5

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

//Misurements
#define CENTRED 100 //distanza del robot rispetto al muro per essere centrato
#define CELL 200

#endif
