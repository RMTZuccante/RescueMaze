#include "definitions.h"
#include "Moviment.h"

Moviment mov(60000);

void setup() {
	mov.begin();
}

void loop() {
  
 for(int i = 0; i<4; i++){
    mov.rotate(true);
  }

  delay(5000);
}
