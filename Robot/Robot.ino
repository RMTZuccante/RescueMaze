#include "definitions.h"
#include "Debug.h"
#include "Robot.h"
#include "Matrix.h"

SerialDebug Debug;
SoftWire I2C_1(PB6, PB7, SOFT_STANDARD);
TwoWire I2C_2(2);

Robot robot;
Matrix matrix;

void pause() {
  matrix.checkpoint();
  nvic_sys_reset();
}

void setup() {
  //I/O initialization
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSHBUTTON, INPUT_PULLUP);
  pinMode(RASP1, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);

  //Hardware initialization
  Serial.begin(115200);
  I2C_1.begin();
  I2C_2.begin();
  robot.setup();
  Debug.println("Hardware initialization done.", LVL_INFO);

  //Check that everything is working
  bool ok = matrix.check() && robot.check() && robot.checkBattery();
  Debug.println("Check done.", LVL_INFO);
  if(!ok) Debug.println("Something is not working correctly. Proceed at your own risk!",LVL_WARN);
  digitalWrite(LED_BUILTIN, ok);
  
  //Software initialization
  robot.begin();
  Debug.setLevel(matrix.getDebug());
  Debug.println("Software initialization done.", LVL_INFO);

  //Waiting user start command
  Debug.println("Waiting for the user to press the button...", LVL_INFO);
  while(digitalRead(PUSHBUTTON));

  //Attaching interrupts
  attachInterrupt(PUSHBUTTON, pause, FALLING);
  attachInterrupt(RASP1, nvic_sys_reset, FALLING);
  
  Debug.println("STARTING!", LVL_INFO);

  if(matrix.wasPaused()) {
    while(!matrix.isOriented(robot.read())) {
      robot.rotate(false);
    }
  }
  
}

void loop() {
  matrix.update(robot.read());
  
  if (matrix.end()) {
    matrix.die();
    digitalWrite(LED_BUILTIN, LOW);
    while(1);    
  }
  
  else if (matrix.black) {
    robot.back();
    matrix.move(false);
  } 
  
  else {
    switch (matrix.getDir()) {
      case RIGHT:
        robot.rotate(false);
        matrix.update(robot.read());
        break;
      case LEFT :
        robot.rotate(true);
        matrix.update(robot.read());
        break;
      case BACK :
        robot.rotate(false);
        robot.rotate(false);
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
