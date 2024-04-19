/*
 * Query the status and position of the arm
 * RS485 baud rate 38400
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
  Serial.print("time: ");
  Serial.print(millis());	// Print time stamp
	
  Serial.print(" state: ");
  Serial.print(mirobot1.getState());  // Get device state
  Serial.print("-");
  Serial.print(mirobot1.getStateToStr()); // Gets device state in string format
	
	// Get all status information at once. This saves time
	STATUS_MIROBOT status = mirobot1.getStatus();
	Serial.print(" state: ");
	Serial.print(status.state);
	Serial.print(" x: ");
	Serial.print(status.cartesian[0]);
	Serial.print(" y: ");
	Serial.print(status.cartesian[1]);
	Serial.print(" z: ");
	Serial.print(status.cartesian[2]);
	Serial.println();
  delay(2000);
}
