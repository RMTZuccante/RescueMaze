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
  //if (!(robot.check() && matrix.check())) while (1);
  digitalWrite(PC13, HIGH);

  //Sensors initialization
  robot.begin();
}

void loop() {
  Serial.println("rotate");
  robot.rotate(false);
  delay(2000);
}
