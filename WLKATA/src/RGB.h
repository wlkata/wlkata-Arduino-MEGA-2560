#ifndef RGB_H
#define RGB_H

#include "Arduino.h"

enum COLOR{
	RGB_BLACK, RGB_WHITE, 
	RGB_RED, RGB_GREEN, RGB_BLUE, 
	RGB_YELLOW, RGB_ORANGE, RGB_VIOLET
};

void RGB_init(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin, bool flag=false);// RGB初始化
void RGB_color(uint8_t _color);// 设置RGB颜色
void RGB_test(uint16_t time);// RGB测试

#endif
