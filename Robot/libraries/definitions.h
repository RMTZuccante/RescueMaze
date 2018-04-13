#ifndef definitions_h
#define definitions_h

//Moviment
#define SPEED 20000
#define FIRST_K 5000
#define SECOND_K 5000
#define ROTATION_P 1500
#define ROTATION_SPEED 60000
#define ROTATION_K 1
#define CLIMB_K 100

//Measurements
#define CENTRED 60 //distance of the robot from the wall to be centered
#define CENTRED2 100
#define CELL_DIM 320
#define CELL 200
#define LASER_DIF -1
#define TEMP_DIF 1.0
#define TEMP_K 23.0

//Color
#define MERROR 70
#define MCOLOR 35

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
#define L_LEFT 0x2A
#define L_FRONTL 0x2B
#define L_RIGHT 0x2C
#define L_FRONTR 0x2D
#define L_BACK 0x2E

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
#define LX_BACK PB4

//Cagamattoni
#define CAGA1 9
#define CAGA2 8
#define CAGADEL 1000
#define CAGADIST 30
#define MINDEL 40

//RGB
#define RED PB15
#define GREEN PB14
#define BLUE PB13

//Battery
#define B_PIN PA4
#define B_R1 68000
#define B_R2 22000

//Interrupts
#define PUSHBUTTON PB3
#define RASP1 PB4
#define RASP2 PB5

#endif
