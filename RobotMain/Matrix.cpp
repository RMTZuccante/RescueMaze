#include "Matrix.h"

/**
 * Check if the Raspberry with the software is running and correctly answering.
 * @return TRUE if the communication works.
 */
bool Matrix::check() {
  int c = 0;
  String m;
  do {
    if (c > 10) return false;
    m = readLine();
    Serial.println(m);
    m = readLine();
    c++;
  } while (m != "ok");
  return true;
}

/**
 * Asks the raspberry which debug level to use.
 * @return Debug level to use.
 */
int Matrix::getDebug() {
  Serial.println("debuglevel");
  return readLine()[0] - '0';
}

/**
 * 
 */

void Matrix::sendRise(bool rise){
  Serial.print("check ");
  Serial.print(300);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.println(rise ? 30 : -30);
}
 
/**
 * Sends the data given to the Raspberry.
 * @param data A data package.
 */
void Matrix::update(RobotData *data) {
  inspect(data);
  getInfo();
}

/**
 * Tells the Raspberry that the robot moved.
 * @param forward TRUE if forward, FALSE if backward.
 */
void Matrix::move(bool forward) {
  Serial.print("move ");
  Serial.println(forward ? "forth" : "back");
}

/**
 * Tells the Raspberry that the robot found a black cell.
 */
void Matrix::sendBlack() {
  Serial.print("check ");
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(2);
  Serial.print(' ');
  Serial.println(0);
}

/**
 * Tells the raspberry that the robot is being manually moved to the nearest checkpoint.
 */
void Matrix::checkpoint() {
  Serial.println("checkpoint");
}

/**
 * Tells the Raspberry to go back to the start of the maze.
 */
void Matrix::backToStart() {
  Serial.println("tostart");
}

/**
 * Tells the Raspberry that the maze is finished.
 */
void Matrix::die() {
  Serial.println("stop");
}

/**
 * Asks the Raspberry if the robot was previously paused.
 * @return TRUE if the robot was paused.
 */
bool Matrix::wasPaused() {
  Serial.println("paused");
  return readLine()[0] - '0';
}

/**
 * Asks the Raspberry if the robot is correctly oriented, to correct it after a pause.
 * @return TRUE if the robot is correctly oriented.
 */
bool Matrix::isOriented(RobotData* data) {
  Serial.print(data->dist[FRONT]);
  Serial.print(' ');
  Serial.print(data->dist[RIGHT]);
  Serial.print(' ');
  Serial.println(data->dist[LEFT]);
  return readLine()[0] - '0';
}

/**
 * Asks the Raspberry about the direction to go.
 * @return 0 to go forward, 1 to turn right, 2 to turn left and 3 to go back.
 */
int Matrix::getDir() {
  Serial.println("getdir");
  return readLine().toInt();
}

/**
 * Asks the Raspberry if the robot has reached the end of the maze.
 * @return TRUE if the robot is at the end.
 */
bool Matrix::end() {
  Serial.println("end");
  return readLine()[0] - '0';
}

/**
 * Sends to the Raspberry the data given.
 * @param data A package containing every sensor data.
 */
void Matrix::inspect(RobotData *data) {
  Serial.print("check ");
  Serial.print(data->dist[FRONT]);
  Serial.print(' ');
  Serial.print(data->dist[RIGHT]);
  Serial.print(' ');
  Serial.print(data->dist[LEFT]);
  Serial.print(' ');
  Serial.print(data->tempL);
  Serial.print(' ');
  Serial.print(data->tempR);
  Serial.print(' ');
  Serial.print(data->color);
  Serial.print(' ');
  Serial.println(data->pitch);
}

/**
 * Asks the raspberry for information about the actual cell.
 */
void Matrix::getInfo() {
  Serial.println("getinfo");
  String s = readLine();
  black = s[0] - '0';
  victim = s[2]  - '0';
}

/**
 * Reads a single line from the Raspberry.
 * @return The line read.
 */
String Matrix::readLine() {
  while (!Serial.available());
  String s;
  char c = Serial.read();
  do {
    s += c;
    while (!Serial.available());
    c = Serial.read();
  } while (c != '\n');
  return s;
}
