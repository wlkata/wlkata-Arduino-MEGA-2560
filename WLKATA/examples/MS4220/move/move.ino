/*
   Move to the specified position, or keep moving.
   Note1: Maximum speed 100.
   Note 2: Most stepper motors are 200step per turn.
*/
#include "WLKATA.h"

MS4220_UART motor1;

void rs485_error(String sendStr, int address) {
  Serial.println("Unable to connect to device " + String(address));
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nInit");

  RS485.begin(38400);	// Set baud rate
  RS485.setOutTimeFunction(rs485_error); // Set the communication timeout period and callback function
  motor1.init(10);  // Set device address
  motor1.reset();	// Stop moving
  delay(3000);// Wait to start

  Serial.println("Start");

}
void loop() {
  // Move to the specified position
  int16_t pos = 0; // unit step
  pos	= 0;
  Serial.println("Move to " + String(pos));
  motor1.moveP(pos, 50); // Move to absolute position 0 at 50% speed
  motor1.waitIdle();// Waiting for idle state

  pos	= 200;
  Serial.println("Move to " + String(pos));
  motor1.moveP(pos, 50); // Move to absolute position 200 at 50% speed
  motor1.waitIdle();// Waiting for idle state

  pos	= -100;
  Serial.println("Move to " + String(pos));
  motor1.moveP(pos, 50); // Move to absolute position -100 at 50% speed
  motor1.waitIdle();// Waiting for idle state


  // Keep moving
  Serial.println("CW");
  motor1.moveAlway(20); // Rotate clockwise at 20% speed.
  delay(3000);

  Serial.println("CCW");
  motor1.moveAlway(-20); // Rotate counterclockwise at 20% speed
  delay(3000);

  Serial.println("STOP");
  motor1.moveAlway(0); // Stop moving
  delay(3000);
}
