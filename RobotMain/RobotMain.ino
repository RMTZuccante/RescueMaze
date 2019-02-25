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
  EMPTY, HANDSHAKE, ROTATE, GO, GETDISTANCES, GETCOLOR, GETTEMPS, VICTIM, READY
};

void reset() {
  nvic_sys_reset();
}

bool handshake() {
  boolean connected = false;
  while (!connected) {
    SerialCom::waitForSerial();
    if (Serial.read() == HANDSHAKE) {
      Serial.write(Serial.read() * 2);
      connected = true;
    }
    else while (Serial.available()) Serial.read();
    return connected;
  }
}

void setup() {
  //I/O initialization
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSHBUTTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);

  //Hardware initialization
  Serial.begin(115200);

  Debug.setLevel(LVL_OFF);
  
  I2C_1.begin();
  I2C_2.begin();
  robot.setup();

  //Check that everything is working
  bool ok = handshake() && robot.check() && robot.checkBattery();
  Debug.println("Check done.", LVL_INFO);
  if (!ok) Debug.println("Something is not working correctly. Proceed at your own risk!", LVL_WARN);

  //I2C::scan(&I2C_1);
  //I2C::scan(&I2C_2);

  digitalWrite(LED_BUILTIN, ok);
  robot.setLED(!ok, 0, 0);

  //Software initialization
  robot.begin();
  Debug.println("Software initialization done.", LVL_INFO);

  //Waiting user start command
  Debug.println("Waiting for the user to press the button...", LVL_INFO);
  robot.setLED(0, 1, 0);
  while (digitalRead(PUSHBUTTON));
  delay(250);
  robot.setLED(0, 0, 0);

  //Attaching interrupts
  attachInterrupt(PUSHBUTTON, reset, FALLING);

  Serial.write(READY);
  Debug.println("STARTING!", LVL_INFO);
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
  switch (Serial.read()) {
    case ROTATE:
      receiveRotate();
      break;
    case GO:
      Serial.write(robot.go());
      break;
    case GETDISTANCES:
      for (int i = 0; i < 5; i++) SerialCom::writeInt(robot.getDistance(i));
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
