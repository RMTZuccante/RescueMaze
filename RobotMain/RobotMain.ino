#include <Arduino.h>
#include "definitions.h"
#include "I2C.h"
#include "Debug.h"
#include "Robot.h"
#include "SerialCom.h"

SerialDebug Debug;
SerialCom Com;

SoftWire I2C_1(PB10, PB11, SOFT_STANDARD);
SoftWire I2C_2(PB8, PB9, SOFT_STANDARD);

Robot robot;

void reset() {
  nvic_sys_reset();
}

void setup() {
  //I/O initialization
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSHBUTTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);

  //Hardware initialization
  Com.begin();
  I2C_1.begin();
  I2C_2.begin();
  robot.setup();

  //Check that everything is working
  bool ok = Com.check() && robot.check() && robot.checkBattery();
  Debug.println("Check done.", Levels::INFO);
  if (!ok) Debug.println("Something is not working correctly. Proceed at your own risk!", Levels::WARN);

  //I2C::scan(&I2C_1);
  //I2C::scan(&I2C_2);

  digitalWrite(LED_BUILTIN, ok);
  robot.setLED(!ok, 0, 0);

  //Software initialization
  robot.begin();
  Debug.println("Software initialization done.", Levels::INFO);

  //Waiting user start command
  Debug.println("Waiting for the user to press the button...", Levels::INFO);
  robot.setLED(0, 1, 0);
  while (digitalRead(PUSHBUTTON));
  delay(250);
  robot.setLED(0, 0, 0);

  //Attaching interrupts
  attachInterrupt(PUSHBUTTON, reset, FALLING);

  Com.writeReady();
  Debug.println("STARTING!", Levels::INFO);
}

void receiveRotate() {
  Com.waitForSerial();
  bool dir = Serial.read();
  Com.waitForSerial();
  byte angle = Serial.read();
  robot.rotate(angle, dir);
  Com.writeRes(1);
}

void loop() {
  Debug.println("Waiting for command");
  Com.waitForSerial();
  switch (Com.getCommand()) {
    case Commands::ROTATE:
      Debug.println("Rotate");
      receiveRotate();
      break;
    case Commands::GO:
      Debug.println("Go");
      Com.writeRes(robot.go());
      break;
    case Commands::GETDISTANCES:
      for (int i = 0; i < 5; i++) Com.writeInt(robot.getDistance(i));
      break;
    case Commands::GETCOLOR:
      Serial.write(robot.getColor());
      break;
    case Commands::GETTEMPS:
      Com.writeFloat(robot.getTempLeft());
      Com.writeFloat(robot.getTempRight());
      break;
    case Commands::VICTIM:
      robot.victim(Serial.read());
      Com.writeRes(1);
      break;
  }
}
