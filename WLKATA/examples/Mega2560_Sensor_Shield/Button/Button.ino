/*
 * Onboard switch routine.
 * If SW1 is triggered, a message will be printed.
 * If SW2 is triggered, arduino will restart.
 * SW1 -> D3
 * SW2 -> D2
 */
#include "WLKATA.h"

void restart() {
  reset();  // arduino reset
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nStart");
  
  shieldInit();
  
  attachInterrupt(digitalPinToInterrupt(PIN_SW2), restart, FALLING);// Configure pin interrupt
}
void loop() {
  if(digitalRead(PIN_SW1) == 0){
    while(digitalRead(PIN_SW1) == 0);
    Serial.println("SW1 triggered");
  }
}
