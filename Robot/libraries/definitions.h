#ifndef definitions_h
#define definitions_h

//Settings
#define SPEED 20000
#define MERROR 5
#define MCOLOR 15


//Temperature adresses
#define T_LEFT 0x5A
#define T_RIGHT 0x5B

//Laser addresses
#define L_LEFT 0x29
#define L_FRONTL 0x2A
#define L_RIGHT 0x2B
#define L_FRONTR 0x2C

//Laser pin
#define LX_LEFT PA15
#define LX_FRONTL PA5
#define LX_FRONTR PA10

//RGB
#define RED PB13
#define GREEN PB14
#define BLUE PB15

//Misurements
#define CENTRED 60 //distanza del robot rispetto al muro per essere centrato
#define CELL 200
#define LASER_FL 10

#endif
