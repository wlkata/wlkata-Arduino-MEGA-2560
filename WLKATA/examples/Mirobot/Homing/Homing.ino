/*
 * 
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
	
  // The default homing does not include a 6th axis and an extension axis
  Serial.println("Device homing ...");
  mirobot1.homing();
  while(mirobot1.getState() != Idle);	// Wait for homing to end
  Serial.println("End of homing");

/*
  // Only the 6th axis homing
  // Note: Some versions of the device do not have a sixth axis sensor and will fail home
  Serial.println("AxisEX homing ...");
  mirobot1.homing(AxisEX);
  while(mirobot1.getState() != Idle); // Wait for homing to end
  Serial.println("AxisEX homing is complete");
*/

/*
  // Only the expansion axis homing
  // Note: If the extension axis homing, the motor and limit sensor must be connected
  Serial.println("Axis6 homing ...");
  mirobot1.homing(Axis6);
  while(mirobot1.getState() != Idle); // Wait for homing to end
  Serial.println("Axis6 homing is complete");
*/
}
void loop() {
}
