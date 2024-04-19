/*
 * Run the action file in the multifunctional extender box.
 * Note: The test.gcode file must be present in the multifunctional extender box.
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
	
	Serial.println("Run file test");
	mirobot1.runFile("test");// Run the test.gcode file
  mirobot1.waitIdle();
	Serial.println("Run out");
}

void loop() {

}
