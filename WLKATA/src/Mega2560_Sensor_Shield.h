#ifndef Mega2560_Sensor_Shield_H
#define Mega2560_Sensor_Shield_H

#include <Arduino.h>
#include "RGB.h"
#include <SPI.h>
// #include <SD.h>

#define PIN_SW1 3//按键引脚
#define PIN_SW2 2//按键引脚

#define PIN_RGB_R 58//RGB红灯引脚
#define PIN_RGB_G 57//RGB绿灯引脚
#define PIN_RGB_B 59//RGB蓝灯引脚

#define PIN_BUZZER 60//蜂鸣器引脚

#define PIN_CS 53//CS引脚

void shieldInit();//扩展板初始化
bool shieldSwitchRead(uint8_t pin);//读按钮电平
void shieldBuzzerWrite(bool val);//写蜂鸣器电平
void shieldBuzzer(uint8_t mode);//播放特定类型的蜂鸣器声音
void reset();//arduino板子重启

#endif
