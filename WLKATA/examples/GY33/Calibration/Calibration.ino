#include "GY33.h"

GY33 gy33;

void setup() {
  Serial.begin(115200);
	Serial2.begin(9600);
  Serial.println();
  Serial.println("Start");
	
  gy33.init(&Serial3);	// Set serial port number
	gy33.setSerialMonitor(1, &Serial);	// Set the serial port number of the monitor
	
	gy33.setCalibration();	// Calibration color
	Serial.println("End of calibration");
	delay(500);
}

void loop() {
	//Reading RGB values
	RGBVAL rgbVal = gy33.readRGB();
	if(rgbVal.r != -1){
		Serial.print("R: ");
		Serial.print(rgbVal.r);
		Serial.print(" G: ");
		Serial.print(rgbVal.g);
		Serial.print(" B: ");
		Serial.println(rgbVal.b);
	}
	else{ 
		Serial.println("Failed to read the RGB value");
	}

}
