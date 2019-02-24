#include <Arduino.h>
#include "definitions.h"
#include "I2C.h"
#include "Debug.h"
#include "Robot.h"
#include "SerialCom.h"

SerialDebug Debug;
SoftWire I2C_1(PB10, PB11, SOFT_STANDARD);
SoftWire I2C_2(PB8, PB9, SOFT_STANDARD);

Robot robot;

enum Commands {
	EMPTY, HANDSHAKE, ROTATE, GO, GETDISTANCES, GETCOLOR, GETTEMPS, VICTIM
};

void pause() {
  //TODO signal checkpoint
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
  bool ok = robot.check() && robot.checkBattery();
  Debug.println("Check done.", LVL_INFO);
  if (!ok) Debug.println("Something is not working correctly. Proceed at your own risk!", LVL_WARN);

  //I2C::scan(&I2C_1);
  //I2C::scan(&I2C_2);  
  
  digitalWrite(LED_BUILTIN, ok);
  robot.setLED(!ok, 0, 0);

  //Software initialization
  robot.begin();
  //TODO Debug.setLevel(matrix.getDebug());
  Debug.println("Software initialization done.", LVL_INFO);

  //Waiting user start command
  Debug.println("Waiting for the user to press the button...", LVL_INFO);
  robot.setLED(0, 1, 0);
  while (digitalRead(PUSHBUTTON));
  delay(250);
  robot.setLED(0, 0, 0);

  //Attaching interrupts
  attachInterrupt(PUSHBUTTON, pause, FALLING);

  Debug.println("STARTING!", LVL_INFO);
  /* TODO
  if (matrix.wasPaused()) {
    while (!matrix.isOriented(robot.read())) {
      robot.rotate(false);
    }
  }*/

}

void sendDistances() {
  uint16_t* arr = robot.getDistances();
  for(int i = 0; i<5; i++) SerialCom::writeInt(arr[i]);
}

void receiveRotate() {
    SerialCom::waitForSerial();
    bool dir = Serial.read();
    SerialCom::waitForSerial();
    byte angle = Serial.read();
    robot.rotate(angle, dir);
    Serial.write((byte)1);
}

void loop() {
  SerialCom::waitForSerial();
  switch(Serial.read()) {
    case HANDSHAKE:
      SerialCom::waitForSerial();
      Serial.write(Serial.read()*2);
      break;
    case ROTATE:
      receiveRotate();
      break;
    case GO:
      Serial.write(robot.go());
      break;
    case GETDISTANCES:
      sendDistances();
      break;
    case GETCOLOR:
      Serial.write(robot.getColor());
      break;
    case GETTEMPS:
      SerialCom::writeFloat(robot.getTempLeft());
      SerialCom::writeFloat(robot.getTempRight());
      break;
    case VICTIM:
      robot.victim(Serial.read());
      Serial.write((byte)1);
      break;
  }
}
