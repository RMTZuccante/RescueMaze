#ifndef definitions_h
#define definitions_h

//Laser positions
#define LASER_FL 0
#define LASER_FR 3
#define LASER_R 1
#define LASER_L  2
#define LASER_B 4

//Moviment
#define BASIC 0
#define BACK_WALL 1
#define FRONT_WALL 2
#define MAXSPEED 65535
#define SPEED 20000
#define FIRST_K 5000
#define SECOND_K 5000
#define ROTATION_P 500
#define SLOW_ROTATION 20000
#define ROTATION_SPEED 60000
#define ROTATION_K 1
#define CLIMB_K 100

//Robot
#define LONG_RANGE 1000
#define BLACK 1
#define OBSTACLE 2
#define RISE 3
#define DISTORTION 30
//Measurements
#define CENTRED 70 //distance of the robot from the wall to be centered
#define CENTRED_BACK 30
#define CENTRED2 80
#define LONG_RANGE 1000
#define CELL_DIM 300
#define CELL_FRONT 200
#define CELL_BACK 100
#define LASER_DIF -10
#define TEMP_DIF 1.0
#define TEMP_K 23

//Temperature addresses
#define T_LEFT 0x5A
#define T_RIGHT 0x5B

//Laser addresses
#define L_LEFT 0x2A
#define L_FRONTL 0x2B
#define L_RIGHT 0x2C
#define L_FRONTR 0x2D
#define L_BACK 0x2E

//Motors pins
#define FR_IN1 PA3
#define FR_IN2 PA4
#define FL_IN1 PB14
#define FL_IN2 PB15

#define FR_EN PA1
#define RL_EN PA8
#define FL_EN PA9
#define RR_EN PA2

//Laser pins
#define LX_FRONTR PB12
#define LX_LEFT PB13
#define LX_RIGHT PA5
#define LX_BACK PA0

//RGB
#define RED PB4
#define GREEN PB3
#define BLUE PA15

//Battery
#define B_PIN PA6
#define B_R1 180000
#define B_R2 68000

//Buttons
#define PUSHBUTTON PC15
#define USBBUTTON PB5

//Cagamattoni
#define CAGAPIN PA10

#endif
