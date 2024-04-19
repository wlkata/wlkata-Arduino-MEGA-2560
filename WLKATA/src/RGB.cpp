#include "RGB.h"

uint8_t color_R, color_G, color_B;
uint8_t pin_rgb_r, pin_rgb_g, pin_rgb_b;
bool levelReversal_flag;

/*
  @说明	RGB init
  @参数	r_pin		Red light pin
	@参数	g_pin		Green light pin
	@参数	b_pin		Blue light pin
  @参数	inverse	Level reversal
  @返回 无
*/
void RGB_init(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin, bool inverse){
	pin_rgb_r = r_pin;
	pin_rgb_g = g_pin;
	pin_rgb_b = b_pin;
	
  pinMode(pin_rgb_r, OUTPUT);
	pinMode(pin_rgb_g, OUTPUT);
	pinMode(pin_rgb_b, OUTPUT);
	
	levelReversal_flag = inverse;
	RGB_color(RGB_RED);
}

/*
  @说明	设置RGB颜色
  @参数	_color	灯的颜色
  @返回 无
*/
void RGB_color(uint8_t _color){
  switch(_color){
    case RGB_BLACK:
      color_R=0, color_G=0, color_B=0; 
      break;
    case RGB_WHITE:
      color_R=1, color_G=1, color_B=1; 
      break;
    case RGB_RED:
			color_R=1, color_G=0, color_B=0; 
      break;
    case RGB_GREEN:
      color_R=0, color_G=1, color_B=0; 
      break;
    case RGB_BLUE:
      color_R=0, color_G=0, color_B=1; 
      break;

    case RGB_ORANGE:
      color_R=1, color_G=0, color_B=1; 
      break;
    case RGB_YELLOW:
      color_R=1, color_G=1, color_B=0; 
      break;
    case RGB_VIOLET:
      color_R=0, color_G=1, color_B=1; 
      break;
  }
	
	if(!levelReversal_flag){
		digitalWrite(pin_rgb_r, color_R);
		digitalWrite(pin_rgb_g, color_G);
		digitalWrite(pin_rgb_b, color_B);
	}
	else{
		digitalWrite(pin_rgb_r, !color_R);
		digitalWrite(pin_rgb_g, !color_G);
		digitalWrite(pin_rgb_b, !color_B);
	}
}

// RGB测试
void RGB_test(uint16_t time){
	RGB_color();
	delay(time);
	RGB_color(RGB_GREEN);
	delay(time);
	RGB_color(RGB_BLUE);
	delay(time);
}

