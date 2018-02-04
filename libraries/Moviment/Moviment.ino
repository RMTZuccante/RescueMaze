#include "Moviment.h"

Moviment mov(10000);

void setup() {
	mov.begin();
	for(int i = 0; i<4; i++){
		mov.rotate(true);
	}
}

void loop() {

}
