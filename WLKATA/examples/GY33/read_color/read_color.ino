#include "GY33.h"

GY33 gy33;

void setup() {
  Serial.begin(115200);
	Serial2.begin(9600);
  Serial.println();
  Serial.println("Start");
	
  gy33.init(&Serial3);	// Set serial port number
	gy33.setSerialMonitor(1, &Serial2);	// Set the serial port number of the monitor
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
	
	//Read color num
	uint8_t color = gy33.readColor();
	if(color != -1){
		Serial.print("RGB num: ");
		Serial.println(color);
	}
	else{ 
		Serial.println("Failed to read the color num");
	}
	
	//Read color name
	String str = gy33.readColorStr();
	if(str != "\0"){
		Serial.print("RGB name: ");
		Serial.println(str);
	}
	else{ 
		Serial.println("Failed to read the color name");
	}
	
	Serial.println();
	delay(1000);
}
