/*
 * Enable the serial port monitor and print communication data on the serial port.
 */
#include "WLKATA.h"

Mirobot_UART mirobot1;

void rs485_error(String sendStr, int address) {
  Serial.print("Unable to connect to device " + String(address));
}

void monitor(String str, bool sendFlag) {
	if(sendFlag)
		Serial.print("	RS485 send: " + str);
	else
		Serial.print("	RS485 recv: " + str);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nInit");
  
	RS485.begin(38400);	// Set baud rate
	RS485.setOutTimeFunction(rs485_error); // Set the communication timeout period and callback function
  RS485.setMonitorFunction(monitor); // Set the monitor call function
	mirobot1.init(1);  // Set device address
	
	Serial.println("Start");
	
  // Device homing if the status is Alarm
  if(mirobot1.getState() == Alarm){
    Serial.println("Device homing ...");
    mirobot1.homing();
  }

  // Wait for homing to end
  while(mirobot1.getState() != Idle) {
    delay(2000);
  }
  
  Serial.println("Device homing is complete");
}

void loop() {

}
