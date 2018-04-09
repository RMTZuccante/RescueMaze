#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include "Robot.h"

#define FRONT 0
#define RIGHT 1
#define LEFT 2
#define BACK 3

class Matrix {
  public:
    bool check();
    int getDebug();

    void update(RobotData *data);

    void move(bool forward);
    void sendBlack();
    void checkpoint();
    void backToStart();
    void die();
    
    bool wasPaused();
    bool isOriented(RobotData* data);
    int getDir();
    bool end();

    bool victim;
    bool black;
  private:
    void inspect(RobotData *data);
    void getInfo();
    String readLine();
};
#endif
