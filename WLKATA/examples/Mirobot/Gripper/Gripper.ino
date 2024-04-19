/*
 * Control Gripper to open and close.
 * Note: A Gripper needs to be connected.
 * Note: The Gripper is in the tightened state for a long time, 
 * which is easy to be damaged by high temperature. 
 * It should be set to off when not in use.
 */
#include "WLKATA.h"

Mirobot_UART mirobot1;

void rs485_error(String sendStr, int address) {
  Serial.println("Unable to connect to device " + String(address));
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nInit");
  
	RS485.begin(38400);	// Set baud rate
	RS485.setOutTimeFunction(rs485_error); // Set the communication timeout period and callback function
  mirobot1.init(1);  // Set device address
	
	Serial.println("Start");
	
  // Device homing if the status is Alarm
  if(mirobot1.getState() == Alarm){
    Serial.println("Device homing ...");
    mirobot1.homing();
  }
}
void loop() {
  mirobot1.setEndtGripper(OPEN);
  delay(1000);
  mirobot1.setEndtGripper(CLOSE);
  delay(1000);
  mirobot1.setEndtGripper(OFF);// It should be set to off when not in use.
  delay(3000);
}