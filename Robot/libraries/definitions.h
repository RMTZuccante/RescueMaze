#ifndef definitions_h
#define definitions_h

//Moviment
#define SPEED 20000
#define FIRST_K 5000
#define SECOND_K 5000
#define ROTATION_P 1000
#define ROTATION_SPEED 60000
#define ROTATION_K 1

//Measurements
#define CENTRED 60 //distance of the robot from the wall to be centered
#define CELL 200
#define LASER_DIF -1

//Color
#define MERROR 5
#define MCOLOR 15

//Debug
#define LVL_OFF 0
#define LVL_WARN 1
#define LVL_INFO 2
#define LVL_DEBUG 3

//Color address
#define COLORADDRESS 0x42

//Temperature addresses
#define T_LEFT 0x5A
#define T_RIGHT 0x5B

//Laser addresses
#define L_LEFT 0x29
#define L_FRONTL 0x2A
#define L_RIGHT 0x2B
#define L_FRONTR 0x2C

//Motors pins
#define FR_IN1 PA3
#define FR_IN2 PA2
#define RL_IN1 PA6
#define RL_IN2 PA7
#define FL_IN1 PB0
#define FL_IN2 PB1
#define RR_IN1 PA1
#define RR_IN2 PA0

//Laser pins
#define LX_FRONTL PA5
#define LX_LEFT PA10
#define LX_RIGHT PA15

//RGB
#define RED PB13
#define GREEN PB14
#define BLUE PB15

//Battery
#define B_PIN PA4
#define B_R1 68000
#define B_R2 22000

//Interrupts
#define PUSHBUTTON PB3
#define RASP1 PB4
#define RASP2 PB5

#endif
