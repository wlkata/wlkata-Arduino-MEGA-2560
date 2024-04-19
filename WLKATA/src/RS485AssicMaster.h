#ifndef _RS485AssicMaster_H
#define _RS485AssicMaster_H

#include <Arduino.h>

#define ReceiveDelay 100
#define OUTTIME 500

class RS485AssicMaster{
  public:
    RS485AssicMaster(HardwareSerial* hwSerial);//通讯串口 参数：串口指针
		
    void begin(unsigned long baudrate);	//初始化 设置波特率
		void begin(HardwareSerial* hwSerial, unsigned long baudrate);	//初始化 设置端口 波特率
    void beginTransmission(int addr, bool waitAckEN = true);	//发送数据
		void writeData(String str);	//发送数据
    void endTransmission();	//结束发送
		void send(String str, int addr, bool waitAckEN = true);	//发送数据
		void resend();	//发送数据
		uint8_t getAckState();//应答状态
		String receive();	//接收指令
		void setOutTimeFunction(void (*_func)(String, int), uint32_t time = OUTTIME);
		void setOutTime(uint32_t time);
		void setMonitorFunction(void (*_func)(String, bool));
		
  protected:
		HardwareSerial* pSerial;
		unsigned long baudrate;
		uint8_t ackState = 0;//应答状态
		String sendStr;//发送字符串
		String receiveStr;//接收字符串
		int address;//设备地址
		char sendBuf[128];//发送字符缓存区
		char receiveBuf[128];//接收字符缓存区
		uint8_t sendLen;//发送字符长度
		uint8_t receiveLen;//接收字符长度
		uint32_t sendTime;//数据发送时间
		bool noAskFunctionFlag = false;
		void (*outTimeFunc)(String, int);//应答超时的回调函数
		uint32_t ackMaxTime = OUTTIME;//最大应答时间 ms
		bool monitorFunctionFlag = false;
		void (*monitorFunc)(String, bool);//应答超时的回调函数
};

extern RS485AssicMaster RS485;

#endif
