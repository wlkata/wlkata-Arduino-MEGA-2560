/*
 * Control Pump to generate positive pressure, negative pressure and shutdown.
 * Note: A Pneumatic box needs to be connected.
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
  mirobot1.setEndtPump(IN);// Suction air
  delay(2000);
  mirobot1.setEndtPump(OUT);// Blow out air
  delay(2000);
  mirobot1.setEndtPump(OFF);
  delay(3000);
}