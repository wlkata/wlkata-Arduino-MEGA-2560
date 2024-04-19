#include "GY33.h"

GY33::GY33(){
	
}

/*
  @说明	设备初始化
  @参数	p			串口号
				buad	波特率 9600或115200
  @返回 无
*/
void GY33::init(HardwareSerial *p, uint32_t buad){
	pSerial = p;
	
  pSerial->begin(buad);	// 设置波特率
	
  pSerial->write(0XA5);	// 设置输出模式——轮询
  pSerial->write(0X00);    
  pSerial->write(0XA5);
  delay(delayTime);
	
	setBrightness(3);
	
  while(pSerial->read()>=0);	// 清空串口接收缓存
}

/*
  @说明	设置数据监视串口
  @参数	en		监视器使能（0/OFF:关闭 1/ON:开启）
  @参数	*p		串口号
  @返回 无
*/
void GY33::setSerialMonitor(bool en, HardwareSerial *p){
	serialMonitorMode = en;
	monitorSerial = p;
}

/*
  @说明	读取RGB值
  @参数	无
  @返回 RGBVAL	返回rgb三个值 （-1读取失败 0-255对应颜色值）
*/
RGBVAL GY33::readRGB(){
	RGBVAL ret;
  uint8_t _buf[8];
  while(pSerial->read() >= 0);	// 清空串口接收缓存
  pSerial->write(0XA5);	// 查询RGB值
  pSerial->write(0X54);    
  pSerial->write(0XF9);
  
  uint32_t _time = millis();
	//等待数据返回，大约400ms
  while(pSerial->available() == 0){
    if(millis()-_time > overTime) return ret;	// 超时跳出
  }
  
  pSerial->readBytes(_buf, 8);
  if(_buf[0] != 0x5A ) return ret;	// 检查帧头
  else if(checkSum(_buf,7) != _buf[7]) return ret;	// 检查校验
	
	if(serialMonitorMode == true) monitorSerial->write(_buf, 8);	// 监视器发送
  
  ret.r = _buf[4];
  ret.g = _buf[5];
  ret.b = _buf[6];
  
  return ret;
}

/*
  @说明	读取颜色
  @参数	无
  @返回 int		读取颜色值 （-1读取失败 0Red 1Yellow 2Pink 3White 4Black 5Green 6Darkblue 7Blue）
*/
int GY33::readColor(){
	int ret = -1;
  uint8_t _buf[11];
  while(pSerial->read() >= 0);	// 清空串口接收缓存
  pSerial->write(0XA5);	// 查询RGB值
  pSerial->write(0X52);    
  pSerial->write(0XF7);
  
  uint32_t _time = millis();
	// 等待数据返回，大约300ms
  while(pSerial->available() == 0){
    if(millis()-_time > overTime) return ret;	// 超时跳出
  }
  
  pSerial->readBytes(_buf, 11);
  if(_buf[0] != 0x5A ) return ret;	// 检查帧头
  else if(checkSum(_buf,10) != _buf[10]) return ret;	// 检查校验
  
	if(serialMonitorMode == true) monitorSerial->write(_buf, 11);	// 监视器发送
	
	for(uint8_t i=0; i<8; i++){
		if( (0x01<<i)&_buf[9] ){
			ret = i;
			break;
		}
	}
  
  return ret;
}

/*
  @说明	读取颜色 字符串格式
  @参数	无
  @返回 String	以字符串格式读取颜色
*/
String GY33::readColorStr(){
	String ret = "\0";
	
	int color = readColor();	// 读取颜色
	if(color == -1) return ret;	// 错误跳出
	
	const String colorStr[]={"Red", "Yellow", "Pink", "White", "Black", "Green", "Darkblue", "Blue"};
	ret = colorStr[color];

  return ret;
}

/*
  @说明	读取RGB值
  @参数	*buffer		保存值的数组指针
  @返回 bool			读取状态
*/
bool GY33::readRGB(uint8_t *buffer){
  uint8_t _buf[8];
  while(pSerial->read() >= 0);	// 清空串口接收缓存
  pSerial->write(0XA5);	// 查询RGB值
  pSerial->write(0X54);    
  pSerial->write(0XF9);
  
  uint32_t _time = millis();
	//等待数据返回，大约400ms
  while(pSerial->available() == 0){
    if(millis()-_time > overTime) return false;	// 超时跳出
  }
  
  pSerial->readBytes(_buf, 8);
  if(_buf[0] != 0x5A ) return false;	// 检查帧头
  else if(checkSum(_buf,7) != _buf[7]) return false;	// 检查校验
	
	if(serialMonitorMode == true) monitorSerial->write(_buf, 8);	// 监视器发送
  
  buffer[0] = _buf[4];
  buffer[1] = _buf[5];
  buffer[2] = _buf[6];
  
  return true;
}

/*
  @说明	读取颜色
  @参数	color		颜色 0
  @返回 bool		读取状态
*/
bool GY33::readColor(uint8_t *color){
  uint8_t _buf[11];
  while(pSerial->read() >= 0);	// 清空串口接收缓存
  pSerial->write(0XA5);	// 查询RGB值
  pSerial->write(0X52);    
  pSerial->write(0XF7);
  
  uint32_t _time = millis();
	// 等待数据返回，大约300ms
  while(pSerial->available() == 0){
    if(millis()-_time > overTime) return false;	// 超时跳出
  }
  
  pSerial->readBytes(_buf, 11);
  if(_buf[0] != 0x5A ) return false;	// 检查帧头
  else if(checkSum(_buf,10) != _buf[10]) return false;	// 检查校验
  
	if(serialMonitorMode == true) monitorSerial->write(_buf, 11);	// 监视器发送
	
  *color = _buf[9];
  
  return true;
}

/*
  @说明	读取颜色 字符串格式
  @参数	color		颜色 0
  @返回 bool		读取状态
*/
bool GY33::readColorStr(String *str){
  uint8_t _buf[11];
  while(pSerial->read() >= 0);	// 清空串口接收缓存
  pSerial->write(0XA5);	// 查询RGB值
  pSerial->write(0X52);    
  pSerial->write(0XF7);
  
  uint32_t _time = millis();
	// 等待数据返回，大约300ms
  while(pSerial->available() == 0){
    if(millis()-_time > overTime) return false;	// 超时跳出
  }
  
  pSerial->readBytes(_buf, 11);
  if(_buf[0] != 0x5A ) return false;	// 检查帧头
  else if(checkSum(_buf,10) != _buf[10]) return false;	// 检查校验
  
	if(serialMonitorMode == true) monitorSerial->write(_buf, 11);	// 监视器发送
	
	const String colorStr[]={"Red", "Yellow", "Pink", "White", "Black", "Green", "Dark blue", "Blue"};
  
	for(uint8_t i=0; i<8; i++){
		if( (0x01<<i)==_buf[9] ){
			*str = colorStr[i];
			break;
		}
	}
  
  return true;
}

/*
  @说明	设置波特率，重新上电后生效
  @参数	num		灯亮度 0-10 数越小，led越亮
  @返回 无
*/
void GY33::setBuad(uint32_t buad){
	if(buad == 115200){
		pSerial->write(0XA5);
		pSerial->write(0XAF);	// 波特率为 115200
		pSerial->write(0X54);
	}
  else{
		pSerial->write(0XA5);
		pSerial->write(0XAE);	// 波特率为 9600
		pSerial->write(0X53);
	}
  delay(delayTime);
}

/*
  @说明	设置补光灯亮度
  @参数	num		灯亮度 0-10 数越小，led越亮
  @返回 无
*/
void GY33::setBrightness(uint8_t num){
  uint8_t _brightness=0x60+num;
  pSerial->write(0XA5);
  pSerial->write(_brightness);    
  pSerial->write(0XA5+_brightness);
  delay(delayTime);
}

/*
  @说明	设置校准
  @参数	无
  @返回 无
*/
void GY33::setCalibration(){
	setBrightness(6);	// 设置亮度6
	
  pSerial->write(0XA5);
  pSerial->write(0X81);	// 连续模式
  pSerial->write(0X26);
  delay(delayTime);
	
  pSerial->write(0XA5);
  pSerial->write(0XBB);	// 校准
  pSerial->write(0X60);
  delay(500);//保证有时间完成校准
	
	setBrightness(3);	// 设置亮度3
	
  pSerial->write(0XA5);
  pSerial->write(0X00);	// 询问模式
  pSerial->write(0XA5);
  delay(delayTime);
}

/*
  @说明	计算校验和
  @参数	*buffer		数据起始地址指针
				len				数据长度
  @返回 uint8_t		校验和
*/
uint8_t GY33::checkSum(uint8_t *buffer, uint8_t len){
  uint8_t sum=0;
  for(uint8_t i=0;i<len;i++)
    sum+=buffer[i]; 
  return sum;
}
