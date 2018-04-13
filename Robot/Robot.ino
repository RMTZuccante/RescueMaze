#include "definitions.h"
#include "Debug.h"
#include "Robot.h"
#include "Matrix.h"

SerialDebug Debug;
SoftWire I2C_1(PB6, PB7, SOFT_STANDARD);
TwoWire I2C_2(2);

Robot robot;
Matrix matrix;

void scan() {
  byte error, address;
  int nDevices;
  Debug.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++) {
    I2C_1.beginTransmission(address);
    error = I2C_1.endTransmission();
    if (error == 0) {
      Debug.print("I2C device found at address 0x");
      if (address < 16) Debug.print("0");
      Debug.println(String(address, HEX));
      nDevices++;
    }
    else if (error == 4) {
      Debug.print("Unknown error at address 0x");
      if (address < 16) Debug.print("0");
      Debug.println(String(address, HEX));
    }    
  }
  if (nDevices == 0) Debug.println("No I2C devices found");
  else Debug.println("done");
}

void pause() {
  matrix.checkpoint();
  nvic_sys_reset();
}

void setup() {
  //I/O initialization
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSHBUTTON, INPUT_PULLUP);
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

  scan();
  digitalWrite(LED_BUILTIN, ok);
  robot.setLED(!ok,0,0);
  
  //Software initialization
  robot.begin();
  Debug.setLevel(matrix.getDebug());
  Debug.println("Software initialization done.", LVL_INFO);

  //Waiting user start command
  Debug.println("Waiting for the user to press the button...", LVL_INFO);
  robot.setLED(0,1,0);
  while(digitalRead(PUSHBUTTON));
  delay(250);
  robot.setLED(0,0,0);

  //Attaching interrupts
  attachInterrupt(PUSHBUTTON, pause, FALLING);
  
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

    Debug.println(String("Found victims: ")+ matrix.victim);
    if (matrix.victim) robot.victim(matrix.victim);
    
    
    matrix.move(true);

    int res = robot.go();

    switch(res){
      case 1:
      //update matrix with black cell
        matrix.sendBlack();
        matrix.move(false);
        break;
      case 2:
        matrix.sendRise(true);
        break;
      case -2:
        matrix.sendRise(false);
        break;
    }
  }
  delay(1000);
}
