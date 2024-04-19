#ifndef _MS4220_H
#define _MS4220_H

#include <Arduino.h>
#include "base.h"
#include "RS485AssicMaster.h"

#define STATEDELAY 500

struct STATUS_MS4220{
	int state=-1;//运动状态
	int32_t pos;//位置
	int feed;//速度
};

class MS4220_UART{
  public:
    MS4220_UART(RS485AssicMaster *p = &RS485);
    void init(int addr = -1);//通讯串口初始化 参数：串口指针 地址
		void sendMsg(String str, bool askEn=ON);//发送指令
		bool receive();//接收指令
		String getVersions();//获取基本信息
		int getState();//读取运动状态
		String getStateToStr();//读取运动状态 字符串类型
		STATUS_MS4220 getStatus();//读取所有状态
		void waitIdle();//等待空闲状态
		
		void homing();//归航
		void zero();//回零点
		
		void setMoveSpeed(int feed);// 速度模式，设置速度 老旧的
		void moveAlway(int feed);// 速度模式，设置速度
		void setMovePos(int32_t pos, uint16_t f);//位置模式，设置位置、速度 老旧的
		void moveP(int32_t pos, uint8_t f);//绝对位置模式，设置位置、速度
		void moveIncP(int32_t pos, uint8_t f);//相对位置模式，设置位置、速度
		void setPos(int32_t pos);// 设置当前的位置 老旧的
		void setCurrentPos(int32_t pos);// 设置当前的位置
		
		void reset();//设备重启
		
  protected:
		STATUS_MS4220 status;//所有状态信息
		RS485AssicMaster *pSerial;//串口指针
		int address;//设备地址
		uint32_t sendTime;//数据发送时间
		float ratio=1;//减速比脉冲
		String firmwareVer="";//固件版本
		
		bool findNum(String* str, String fStr, uint8_t num, String* retStr);//查找指定字符串后的数据
		bool findState(String* str, char c1, char c2);//查找状态字符串
		bool dealstatus(String* str);//解析返回状态
		
};

#endif
