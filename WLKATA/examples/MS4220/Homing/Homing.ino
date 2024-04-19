/*
 * homing: When the sensor is triggered, the position is 0
 * Note 1: If the indicator is green after power-on, 
 * 		the position is 0 during power-on. 
 * 		In this case, you do not need to use homing.
 * Note2: homing requires an external sensor, if there is no sensor, 
 * 		it will cause constant motion
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
	
  Serial.println("Device homing ...");
  motor1.homing();// homing
  while(motor1.getState() != Idle);	// Wait for homing to end
  Serial.println("End of homing");
}
void loop() {
}
