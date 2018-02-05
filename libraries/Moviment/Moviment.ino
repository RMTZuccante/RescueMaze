#include "definitions.h"
#include "Moviment.h"
Moviment mov(30000);

void setup() {
	mov.begin();
  Serial.begin(115200); 
  while(!Serial.isConnected()){};
  Serial.println("ciao");
  for (int i = 0; i < 10; i++) Serial.println(1);
}

void loop() {
  for(int i =0; i<4;i++){
    for (int i = 0; i < 10; i++) Serial.println(1);
    mov.rotate(true);
    delay(5000);
  }
  
}
