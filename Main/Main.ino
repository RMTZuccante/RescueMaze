#include "definitions.h"
#include "Moviment.h"
#include "Color.h"
#include "Temperature.h"
#include "RGB.h"
//#include "Laser.h"
//#include "Matrix.h"

//Temperature adress
#define T_LEFT 0x5A
#define T_RIGHT 0x5B
//RGB
#define RED 4
#define GREEN 5
#define BLUE 6
//Directions
#define FRONT 0
#define RIGHT 1
#define LEFT 2
#define BACK 3
//Misurements
#define CENTRED 60 //distanza del robot rispetto al muro per essere centrato

//creating objects
Moviment mov(50000);
Color *color;
Temperature tempL = Temperature(T_LEFT);
Temperature tempR = Temperature(T_RIGHT);
RGB led(RED , GREEN , BLUE);
//Laser laser[3]={Laser(),Laser(),Laser()};
//Matrix matrix();


float dist[3];

/*
void getDist(){
  for(int i=0; i<3; i++){
    dist[i]=laser[i].read();
  }//for
}//getDist
*/

/*
void go(){
  mov.go();
  float endDist = endDist(dist[0].read());
  while( (dist[0].read()>endDist) && (color->read()==2) );
  mov.stop;
}//go
*/

/*
void back(){
  mov.go(true);
  float endDist = endDist( dist[0].read() ) + 300;
  while( (dist[0].read()<endDist));
  mov.stop;
}
*/

float endDist(float distance){
  return distance-distance%300+CENTRED;
}

void setup(){
  Serial.begin(9600);
  color = new Color();
  color->begin();
  mov.begin();
  //for(int i = 0 ; i<3 ; i++)laser[i].begin();
  
}//setup

void loop(){
  /*  
   *  matrix._check(dist,tempL.read(),tempR.read(),color->read());
   *  if(matrix.isBlack())back();
   *  if(matrix.isVictim()){
   *    cagamattoni.caga();
   *    RGB.set(0,0,0);
   *  }
   *  switch(matrix.getDir()){
   *    case 1 :
   *      mov.rotate(true);
   *      break;
   *    case 2 :
   *      mov.rotate(false)
   *      break ;
   *  }//switch
   *  go();
   * 
   */
}//loop
