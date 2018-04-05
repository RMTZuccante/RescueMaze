#include "Debug.h"
#include "Robot.h"
#include "Matrix.h"

SerialDebug Debug;
SoftWire I2C_1(PB6, PB7, SOFT_STANDARD);
TwoWire I2C_2(2);

Robot robot;
Matrix matrix;

void setup() {
  //Hardware initialization
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);
  Serial.begin(115200);
  I2C_1.begin();
  I2C_2.begin();
  robot.setup();
  Debug.println("Hardware initialization done.", LVL_INFO);

  //Check that everything is working
  bool ok = matrix.check() && robot.check() && robot.checkBattery();
  Debug.println("Check done.", LVL_INFO);
  if(!ok) Debug.println("Something is not working correctly. Proceed at your own risk!",LVL_WARN);
  digitalWrite(PC13, ok);
  
  //Sensors initialization
  robot.begin();
  Debug.println("Software initialization done.", LVL_INFO);
  Debug.println("STARTING!", LVL_INFO);
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
