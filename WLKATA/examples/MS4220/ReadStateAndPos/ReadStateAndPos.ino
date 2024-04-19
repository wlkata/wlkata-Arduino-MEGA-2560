/*
   Query the status and position
   RS485 baud rate 38400
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
  motor1.reset();  // Stop moving
  delay(3000);
  Serial.println("Start");


}

void loop() {
  Serial.print("time: ");
  Serial.print(millis());	// Print time stamp

  // Get all status information at once. This saves time
  STATUS_MS4220 status = motor1.getStatus();
  Serial.print(" state: ");
  Serial.print(status.state);
  Serial.print(" pos: ");
  Serial.print(status.pos);
  Serial.print(" speed: ");
  Serial.print(status.feed);
  Serial.println();
  delay(2000);
}
