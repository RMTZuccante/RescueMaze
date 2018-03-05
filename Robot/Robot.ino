#include "Robot.h"
#include "Matrix.h"

extern SoftWire I2C_1(PB6, PB7, SOFT_STANDARD);
extern TwoWire I2C_2(2);

Robot robot;
Matrix matrix;

void setup() {
  //Hardware initialization
  Serial.begin(115200);
  I2C_1.begin();
  I2C_2.begin();
  robot.setup();

  //Check that everything is working
  digitalWrite(PC13, LOW);
  if(!(robot.check() && matrix.check())) while(1);
  digitalWrite(PC13, HIGH);
  
  //Sensors initialization
  robot.begin();
}

void loop() {
	robot.update();
    matrix.update(robot.data);
    
    if(matrix.black){
      robot.back();
      matrix.move(false);
    }
    
    if(matrix.victim) robot.victim();
    
    switch(matrix.dir){
      case 1 :
        robot.rotate(true);
        break;
      case 2 :
        robot.rotate(false);
        break;
    }
    robot.go();
}
