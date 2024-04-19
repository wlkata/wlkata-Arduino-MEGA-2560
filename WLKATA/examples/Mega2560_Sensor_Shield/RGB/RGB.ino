/*
 * Make different sounds through the buzzer
 * red   -> D58
 * green -> D57
 * blue  -> D59
 */
#include "WLKATA.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nStart");
  
  shieldInit();
}
void loop() {
  RGB_color(RGB_BLACK);
  delay(1000);
  RGB_color(RGB_RED);
  delay(1000);
  RGB_color(RGB_BLUE);
  delay(1000);
  RGB_color(RGB_GREEN);
  delay(1000);
  RGB_color(RGB_WHITE);
  delay(1000);
  RGB_color(RGB_YELLOW);
  delay(1000);
  RGB_color(RGB_VIOLET);
  delay(3000);
}