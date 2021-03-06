#include <Arduino.h>
#include "definitions.h"
#include "I2C.h"
#include "Debug.h"
#include "Robot.h"
#include "SerialCom.h"

Robot robot;

void reset() {
  nvic_sys_reset();
}

void setup() {
  //I/O initialization
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(USBBUTTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);

  //Hardware initialization
  Com.begin();
  Serial.begin();
  I2C_1.begin();
  I2C_2.begin();

  bool usbMode = !digitalRead(USBBUTTON);

  if(usbMode) {
    pinMode(PUSHBUTTON, INPUT_PULLUP);
    pinMode(PUSHBUTTON2, INPUT_PULLUP);
  } else {  
    pinMode(PUSHBUTTON, INPUT);
    pinMode(PUSHBUTTON2, INPUT);
  }
  
  //Attaching interrupts
  attachInterrupt(PUSHBUTTON2, reset, FALLING);
  
  //Check that everything is working
  bool ok = (usbMode || Com.check()) && robot.check() && robot.checkBattery();
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
  robot.setLED(0, 1, usbMode);
  while (digitalRead(PUSHBUTTON));
  Debug.println("Button has been pushed!");
  delay(250);
  robot.setLED(0, 0, 0);

  //Attaching interrupts
  attachInterrupt(PUSHBUTTON, reset, FALLING);

  robot.calibrate();

  Debug.println("STARTING!", Levels::INFO);

  if(usbMode) while(1) loopUSB();
}

void receiveRotate() {
  bool dir = Com.read();
  byte angle = Com.read();
  robot.rotate(dir, angle);
  Com.notifyRes(1);
}

void sendColor(ColorData cd) {
  Com.write(cd.red);
  Com.write(cd.green);
  Com.write(cd.blue);
  Com.write(cd.ambient);
}

void loop() {
  Com.notifyReady();
  switch (Com.getCommand()) {
    case Commands::ROTATE:
      Debug.println("Rotate");
      receiveRotate();
      break;
    case Commands::GO:
      Debug.println("Go");
      Com.notifyRes(robot.go());
      break;
    case Commands::GETDISTANCES:
      Com.write(Commands::GETDISTANCES);
      for (int i = 0; i < 5; i++) Com.write(robot.getDistance(i));
      break;
    case Commands::GETCOLOR:
      Com.write(Commands::GETCOLOR);
      sendColor(robot.getColor());
      break;
    case Commands::GETTEMPS:
      Com.write(Commands::GETTEMPS);
      Com.write(robot.getTempLeft());
      Com.write(robot.getTempRight());
      Com.write(robot.getTempAmb());
      break;
    case Commands::GETINCLINATION:
      Com.write(Commands::GETINCLINATION);
      Com.write(robot.getInclination());
      break;
    case Commands::VICTIM:
      robot.victim(Com.read());
      Com.notifyRes(1);
      break;
    case Commands::SETDEBUG:
      Debug.setLevel(static_cast<Levels>(Com.read()));
      break;
    case Commands::SETBLACK:
      robot.setBlackThreshold(Com.read());
      break;
    case Commands::MIRROR:
      gpio_toggle_bit(PIN_MAP[LED_BUILTIN].gpio_device, PIN_MAP[LED_BUILTIN].gpio_bit);
      break;
    case Commands::RESET:
      reset();
      break;
  }
}

void waitUSB() {
  while(!Serial.available());
}

void loopUSB() {
  Serial.println("Ready!");
  waitUSB();
  String cmd = Serial.readStringUntil('\n');
  if(cmd == "rotate") {
    Serial.println("Angle?");
    waitUSB();
    robot.rotate(Serial.parseFloat());
  }
  else if (cmd == "go") {
    Serial.println("Go ended with code: "+String(robot.go()));
  }
  else if (cmd == "getdistances") {
    Serial.println(robot.getDistances());
  }
  else if (cmd == "getcolor") {
    Serial.println(robot.getColor());
  }
  else if (cmd == "gettemps") {
    Serial.println(robot.getTemps());
  }
  else if (cmd == "victim") {
    Serial.println("Number of packets?");
    waitUSB();
    robot.victim(Serial.parseInt());
  }
  else if (cmd == "setdebug") {
    Serial.println("Debug level?");
    waitUSB();
    Debug.setLevel(static_cast<Levels>(Serial.parseInt()));
  }
  else if (cmd == "setblack") {
    Serial.println("Black threshold?");
    waitUSB();
    robot.setBlackThreshold(Serial.parseInt());
  }
  else if (cmd == "reset") {
    reset();
  }
  else if (cmd == "getinclination") {
    Serial.println("Inclination: "+String(robot.getInclination()));
  }
  else if (cmd == "getbattery") {
    Serial.println("Battery: "+String(robot.getBattery())+"v.");
  }
  else if (cmd == "mirror") {
      gpio_toggle_bit(PIN_MAP[LED_BUILTIN].gpio_device, PIN_MAP[LED_BUILTIN].gpio_bit);
  }
  else Serial.println("Unknown command");
}
