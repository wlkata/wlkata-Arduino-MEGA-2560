#include "Mega2560_Sensor_Shield.h"

void(* resetFunc)();

/*
  @说明	扩展板初始化，包含RGB、蜂鸣器、按键
  @参数	无
  @返回 无
*/
void shieldInit(){
	RGB_init(PIN_RGB_R, PIN_RGB_G, PIN_RGB_B, 1);//RGB初始化
  
	pinMode(PIN_BUZZER, OUTPUT);
	digitalWrite(PIN_BUZZER, LOW); //蜂鸣器初始化
	
	pinMode(PIN_SW1, INPUT_PULLUP);
	pinMode(PIN_SW2, INPUT_PULLUP);
	
	
}

/*
  @说明	读按钮电平
  @参数	无
  @返回 bool 开关电平 0开关触发 1开关未触发
*/
bool shieldSwitchRead(uint8_t pin){
	return digitalRead(pin);
}

/*
  @说明	写蜂鸣器电平
  @参数	val	蜂鸣器电平 0无声音 1有声音
  @返回 无
*/
void shieldBuzzerWrite(bool val){
	digitalWrite(PIN_BUZZER, val);
}

/*
  @说明	播放特定类型的蜂鸣器声音
  @参数	mode	蜂鸣器声音类型
  @返回 无
*/
void shieldBuzzer(uint8_t mode){
	uint16_t t;
	switch(mode){
		case 0:
			for(uint8_t i=0; i<2; i++){
				digitalWrite(PIN_BUZZER, HIGH);
				delay(100);
				digitalWrite(PIN_BUZZER, LOW);
				delay(100);
			}
			break;
		case 1:
			t=400;
			for(uint8_t j=0; j<3; j++){
				for(uint32_t i=0; i<150000/t; i++){
					digitalWrite(PIN_BUZZER, HIGH);
					delayMicroseconds(t-10);
					digitalWrite(PIN_BUZZER, LOW);
					delayMicroseconds(10);
				}
				t+=400;
			}
			break;
		case 2:
			digitalWrite(PIN_BUZZER, HIGH);
			delay(800);
			digitalWrite(PIN_BUZZER, LOW);
			break;
		case 3:
			digitalWrite(PIN_BUZZER, HIGH);
			delay(100);
			digitalWrite(PIN_BUZZER, LOW);
			break;
		default:
			break;
	}
}

/*
  @说明	arduino板子重启
  @参数	无
  @返回 无
*/
void reset(){
	resetFunc();
}
