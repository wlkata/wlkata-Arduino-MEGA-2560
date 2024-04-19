#ifndef _E4_H
#define _E4_H

#include <Arduino.h>
#include "base.h"
#include "RS485AssicMaster.h"

//路径模式
#define MOVEP 0	//点到点运动
#define MOVEL 1	//直线运动
#define MOVEJ 1	//关节运动
#define JUMP 	2	//门型运动
//运动模式
#define ABS 0	//绝对位置
#define INC 1	//相对位置
//圆弧方向
#define CW 0	//顺时针画弧
#define CCW 1	//逆时针画弧

#define STATEDELAY 500

struct STATUS_E4{
	int state=-1;//运动状态 
	float angle[7];//关节角度
	float cartesian[6];//直角坐标
	int pumpPwm;//pwm输出
};

class E4_UART{
  public:
    E4_UART(RS485AssicMaster *p = &RS485);
    void init(int addr = -1);//通讯串口初始化 参数：串口指针 地址
		void sendMsg(String str, bool askEn=ON);//发送指令
		bool receive();//接收指令
		String getVersions();//获取基本信息
		int getState();//读取运动状态
		String getStateToStr();//读取运动状态 字符串类型
		STATUS_E4 getStatus();//读取所有状态
		void waitIdle();//等待空闲状态
		
		void homing(int homingMode = -1);//归航
		void zero();//回零点
		
		//void move(bool motionMode, float j1, float j2, float j3, float j4, float j5, float j6);//机械臂关节运动
		
		void movePose(uint8_t pathMode, bool motionMode, float x, float y, float z, float a);//机械臂位置运动
		void movePoseWithExj(uint8_t pathMode, bool motionMode, float x, float y, float z, float a, float d);//机械臂位置运动，含扩展轴
		void moveArc(bool pathMode, bool motionMode, float x, float y, float z, float r);//机械臂圆弧插补运动
		void moveJoints(bool motionMode, float j1, float j2, float j3, float j4);//机械臂关节运动

		//void moveP(float x, float y, float z, float a, float b, float c);//点到点运动
		//void moveL(float x, float y, float z, float a, float b, float c);//直线运动
		//void moveJ(float x, float y, float z, float a, float b, float c);//关节运动
		//void jump(float x, float y, float z, float a, float b, float c);//门型轨迹运动
		//void moveIncP(float x, float y, float z, float a, float b, float c);//点到点相对运动
		//void moveIncL(float x, float y, float z, float a, float b, float c);//直线相对运动
		//void moveIncJ(float x, float y, float z, float a, float b, float c);//关节相对运动
		//void jumpInc(float x, float y, float z, float a, float b, float c);//门型轨迹相对运动

		void setMotionSpeed(float speed);//设置运动速度
		void motionSpeedRatio(uint8_t ratio);//设置运动速度百分比
		//void motionParams();//设置运动参数
		
		void movePause();//暂停运动
		void moveContinue();//继续运动
		void moveStop();//停止运动
		
		//扩展轴
		void setExjRatio(float ratio);//设置扩展轴减速比
		void moveExjPulse(bool motionMode, int32_t d);//扩展轴运动-单位脉冲
		void moveExjDist(bool motionMode, float d);//扩展轴运动-单位距离
		
		//末端工具
    void setEndtGripper(uint8_t num);//控制电动夹爪状态
    void setEndtPump(uint8_t num);//控制气泵状态
    void setEndtPwm(uint16_t num);//控制pwm输出脉宽
		
		//void saveParams();//保存参数
		
		void runFile(String fileName, bool loop = false);//运行文件名
		void runFileNum(uint8_t fileNum, bool loop = false);//运行文件号
		
		void reset();//设备重启
		
  protected:
		STATUS_E4 status;//所有状态信息
		RS485AssicMaster *pSerial;//串口指针
		int address;//设备地址
		uint32_t sendTime;//数据发送时间
		float exj_ratio=1;//扩展轴减速比脉冲
		String armVer="\0";//机械臂版本
		String exboxVer="\0";//扩展盒版本
		String versions="\0";//版本
		
		bool findNum(String* str, String fStr, uint8_t num, String* retStr);//查找特定字符串
		bool findState(String* str, char c1, char c2);//查找状态字符串
		bool dealstatus(String* str);//解析返回状态
		
};

#endif
