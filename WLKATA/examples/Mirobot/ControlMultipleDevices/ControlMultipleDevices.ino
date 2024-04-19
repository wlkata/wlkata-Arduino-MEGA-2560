/*
 * Control multiple devices
 */
#include "WLKATA.h"

Mirobot_UART mirobot1;
Mirobot_UART mirobot2;

void rs485_error(String sendStr, int address) {
  Serial.println("Unable to connect to device " + String(address));
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nInit");
  
	RS485.begin(38400);	// Set baud rate
	RS485.setOutTimeFunction(rs485_error); // Set the communication timeout period and callback function
  mirobot1.init(1);  // Set device address
	mirobot2.init(2);  // Set device address
	
	Serial.println("Start");
	
  Serial.println("Mirobot1 homing ...");
  mirobot1.homing();
  while(mirobot1.getState() != Idle);	// Wait for homing to end
  Serial.println("Mirobot1 homing is complete");

  Serial.println("Mirobot2 homing ...");
  mirobot2.homing();
  while(mirobot2.getState() != Idle);  // Wait for homing to end
  Serial.println("Mirobot2 homing is complete");
}
void loop() {
}
