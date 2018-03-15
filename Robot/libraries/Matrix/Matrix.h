#ifndef MATRIX_H
#define MATRIX

#include <Arduino.h>
#include "Robot.h"

#define FRONT 0
#define RIGHT 1
#define LEFT 2
#define BACK 3

class Matrix {
  public:
    bool check();
    void debug(String s);

    void update(RobotData data);

    void move(bool forward);
    void backToStart();
    bool end();
    void die();
    int getDir();

    bool victim;
    bool black;
  private:
    void inspect(RobotData data);
    void getInfo();
    String readLine();
};
#endif
