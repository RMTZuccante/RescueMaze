#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
  public:
    Motor(byte e, byte i1, byte i2);
    void start(byte speed, bool inverse);
    void setSpeed(byte speed);
    void stop();
	void begin();
  private:
    byte inverter1, inverter2, enable;
	bool inverse;
};

/*
   @brief costruttore dell'oggetto motore
*/
Motor::Motor(byte e, byte i1, byte i2) {
  inverter1 = i1;
  inverter2 = i2;
  enable = e;
}


/*
   @brief metodo che muove di un 'passo' una coppia di motori
*/
void Motor::start(byte speed, bool inverse) {
	this->inverse=inverse;
    pwmWrite(enable, speed);
    gpio_write_bit(GPIOB,inverter1, !inverse);
    gpio_write_bit(GPIOB,inverter2, inverse);
}

void Motor::begin(){
  pinMode(inverter1, OUTPUT);
  gpio_write_bit(GPIOB,inverter1, LOW);
  pinMode(inverter2, OUTPUT);
  gpio_write_bit(GPIOB,inverter2, HIGH);
  pinMode(enable, PWM);
  pwmWrite(enable, 0);
}

void Motor::setSpeed(byte speed) {
	start(speed, inverse);
}

void Motor::stop() {
  gpio_write_bit(GPIOB,enable, LOW);
}

#endif
