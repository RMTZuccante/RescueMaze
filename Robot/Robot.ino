#include "Robot.h"
#include "Matrix.h"

extern SoftWire I2C_1(PB6, PB7, SOFT_STANDARD);
extern TwoWire I2C_2(2);

Robot robot;
Matrix matrix;

/**
 * Sets the addresses to the distance sensors.
 */
void setAddresses() {
  pinMode(LX_LEFT, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONTR, OUTPUT_OPEN_DRAIN);
  pinMode(LX_FRONTL, OUTPUT_OPEN_DRAIN);
  digitalWrite(LX_FRONTR, LOW);
  digitalWrite(LX_FRONTL, LOW);
  digitalWrite(LX_LEFT, LOW);
  robot.laser[1].setAddress(L_RIGHT);
  digitalWrite(LX_FRONTL, HIGH);
  delay(10);
  robot.laser[3].setAddress(L_FRONTL);
  digitalWrite(LX_FRONTR, HIGH);
  delay(10);
  robot.laser[0].setAddress(L_FRONTR);
  digitalWrite(LX_LEFT, HIGH);
  delay(10);
  robot.laser[2].setAddress(L_LEFT);
}


void setup() {
  //Hardware initialization
  Serial.begin(115200);
  I2C_1.begin();
  I2C_2.begin();
  
  setAddresses();
  
  //Check that everything is working
  //pinMode(PC13, OUTPUT);
  //digitalWrite(PC13, !( matrix.check() && robot.check()));
  matrix.check();

  //Sensors initialization
  robot.begin();
}

void loop() {
  robot.update();
  matrix.update(robot.data);
  if (matrix.black) {
    robot.back();
    matrix.move(false);
  } else {
    int dir = matrix.getDir();
    switch (dir) {
      case RIGHT:
        robot.rotate(false);
        robot.update();
        matrix.update(robot.data);
        break;
      case LEFT :
        robot.rotate(true);
        robot.update();
        matrix.update(robot.data);
        break;
      case BACK :
        robot.rotate(false);
        //robot.update();
        robot.rotate(false);
        //matrix.update(robot.data);
        break;
    }

    if (matrix.victim) robot.victim();
    matrix.move(true);
    if (!robot.go()) {
      //update matrix with black cell
      matrix.sendBlack();
      matrix.move(false);
    }
  }
}
