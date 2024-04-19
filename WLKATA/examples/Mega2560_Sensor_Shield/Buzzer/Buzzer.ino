/*
 * Make different sounds through the buzzer
 * Buzzer -> D60
 */
#include "WLKATA.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nStart");
  
  shieldInit();
}
void loop() {
  shieldBuzzer(0);// di di
	delay(1000);
  shieldBuzzer(1);// Pitch drop
	delay(1000);
  shieldBuzzer(2);// Prolonged sound
	delay(3000);
}