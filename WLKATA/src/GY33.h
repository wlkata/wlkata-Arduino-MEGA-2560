/*
 *介绍：适用于GY33颜色传感器
 *版本：1.0
*/
#ifndef GY33_H
#define GY33_H

#include "Arduino.h"

struct RGBVAL{
	int r = -1;	//红色
	int g = -1;	//绿色
	int b = -1;	//蓝色
};

class GY33{
  public:
    GY33();
		void init(HardwareSerial *p, uint32_t buad=9600);	// 初始化
		void setSerialMonitor(bool en, HardwareSerial *p);	// 设置数据监视串口
		RGBVAL readRGB();	// 读取RGB值
		int readColor();	// 读取颜色
		String readColorStr();	// 读取颜色
		
		bool readRGB(uint8_t *buffer);	// 读取RGB值
		bool readColor(uint8_t *color);	// 读取颜色
		bool readColorStr(String *str);	// 读取颜色
		void setBuad(uint32_t buad);	// 设置波特率
		void setBrightness(uint8_t num);	// 设置补光灯亮度 参数：0-10 数越小，led越亮
		void setCalibration();	// 设置校准

  protected:
		const uint16_t delayTime = 250;	// 数据之间时间间隔 大于190即可
		const uint16_t overTime = 1000;	// 数据接收超时时间
		HardwareSerial *pSerial;	// 颜色传感器串口号
		bool serialMonitorMode = false;	// 监视器开关
		HardwareSerial *monitorSerial;	// 监视器串口号
		
		uint8_t checkSum(uint8_t *buffer, uint8_t len);	// 计算校验和 参数：数组名称，数据数量
};

#endif
