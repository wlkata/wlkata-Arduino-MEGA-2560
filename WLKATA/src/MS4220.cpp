#include "MS4220.h"

MS4220_UART::MS4220_UART(RS485AssicMaster *p){
	pSerial = p;
}

/*
  @说明	设置的通讯相关参数
  @参数	*p		串口号
  @参数	addr	设备地址，范围：1-247，0为广播地址
							如果该串口只有一个设备，可以不设置地址，或将地址设为-1
  @返回 无
*/
void MS4220_UART::init(int addr){
	address = addr;
}

/*
  @说明	发送指令，可选是否等待应答
  @参数	str		要发送的字符串，不需要带地址，自动添加
  @参数	askEn	应答等待使能（0：不等待应答 1：等待应答）
  @返回 无
*/
void MS4220_UART::sendMsg(String str, bool askEn){
	pSerial->send(str, address, askEn);
	sendTime = millis();
	if(askEn) receive();
}

/*
  @说明 接收指令，可选是否等待应答
  @参数	无
  @返回 bool	接收应答标志（0：应答完成 1：等待应答）
*/
bool MS4220_UART::receive(){
	while(pSerial->getAckState()){
		String str = pSerial->receive();

		if(str.startsWith("<")) dealstatus(&str);
		else if(str.startsWith("MS4220")){
			uint8_t size=str.length();
			firmwareVer = str.substring(0,size-2);
		}
		else if(str.endsWith("ok\r\n"));
		else;
	}
}

/*
  @说明 获取基本信息
  @参数	无
  @返回 String
*/
String MS4220_UART::getVersions(){
	firmwareVer = "\0";
	sendMsg("$v\r\n", 1);
	return firmwareVer + "\r\n";
}

/*
  @说明 读取运动状态
  @参数	无
  @返回 int
*/
int MS4220_UART::getState(){
	getStatus();
	return status.state;
}

/*
  @说明 读取运动状态 字符串类型
  @参数	无
  @返回 String
*/
String MS4220_UART::getStateToStr(){
	while(millis()-sendTime<STATEDELAY);//给发送预留时间
	int num = getState();
	if(num==-1) return "Error";
	return stateToStr(num);
}

/*
  @说明 读取所有状态
  @参数	无
  @返回 状态集
*/
STATUS_MS4220 MS4220_UART::getStatus(){
	while(millis()-sendTime<STATEDELAY);//给发送预留时间
	sendMsg("?\r\n", 1);
	return status;
}

/*
  @说明 等待空闲状态
  @参数	无
  @返回 无
*/
void MS4220_UART::waitIdle(){
	while(getState() != Idle);//等待空闲
}

/*
  @说明 归零
  @参数	无
  @返回 无
*/
void MS4220_UART::homing(){
  sendMsg("$H\r\n", 1);//$H
}

/*
  @说明 移动到零点位置
  @参数	无
  @返回 无
*/
void MS4220_UART::zero(){
  sendMsg("G90 G1 E0\r\n",1);//$H
}

/*
  @说明 速度模式，设置速度
  @参数	feed	运动速度
  @返回 无
*/
void MS4220_UART::setMoveSpeed(int feed){
	String str = "\0";
	str += "G6 F";
	str += String(feed);
	str += "\r\n";
  sendMsg(str,1);
}
void MS4220_UART::moveAlway(int feed){
	String str = "\0";
	str += "G6 F";
	str += String(feed);
	str += "\r\n";
  sendMsg(str,1);
}

/*
  @说明 绝对位置模式，设置位置、速度
  @参数	pos		绝对位置
  @参数	f			运动速度
  @返回 无
*/
void MS4220_UART::setMovePos(int32_t pos, uint16_t f){
	sendMsg("G90 G1 E"+String(pos)+" F"+String(f)+"\r\n",1);
}
void MS4220_UART::moveP(int32_t pos, uint8_t f){
	sendMsg("G90 G1 E"+String(pos)+" F"+String(f)+"\r\n",1);
}

/*
  @说明 相对位置模式，设置位置、速度
  @参数	pos		相对位置
  @参数	f			运动速度
  @返回 无
*/
void MS4220_UART::moveIncP(int32_t pos, uint8_t f){
	sendMsg("G91 G1 E"+String(pos)+" F"+String(f)+"\r\n",1);
}

/*
  @说明 设置当前的位置
  @参数	pos		位置坐标
  @返回 无
*/
void MS4220_UART::setPos(int32_t pos){
	sendMsg("G92 E"+String(pos)+"\r\n",1);
}
void MS4220_UART::setCurrentPos(int32_t pos){
	sendMsg("G92 E"+String(pos)+"\r\n",1);
}

/*
  @说明 设备重启
  @参数	无
  @返回 无
*/
void MS4220_UART::reset(){
	sendMsg("%\r\n", 1);
	delay(200);
	sendMsg("\r\n", 1);
}

/*
  @说明 查找指定字符串后的数据
  @参数	str			状态所在的字符串
	@参数	fStr		要查找的字符串
	@参数	num			寻找字符串后的第几个数据,最小为1
	@参数	retStr	读取到的数据
  @返回 bool 		读取成功标志（0：失败 1：成功）
*/
bool MS4220_UART::findNum(String* str, String fStr, uint8_t num, String* retStr){
	uint8_t startAddr,endAddr;
	startAddr = (*str).indexOf(fStr);// 找指定字符串的位置
	if(startAddr == -1) return 0;
	endAddr = (*str).indexOf(':', startAddr);//找字符':'的位置
	if(endAddr == -1) return 0;
	
	for(uint8_t i=0; i<num; i++){
		startAddr = endAddr+1;
		endAddr = (*str).indexOf(',', startAddr);
		if(endAddr == -1) return 0;
	}
	*retStr = (*str).substring(startAddr, endAddr);
	return 1;
}

/*
  @说明 查找字符串中的运动状态
  @参数	str		状态所在的字符串
	@参数	c1		开始字符
	@参数	c2		结束字符
  @返回 bool 	读取成功标志（0：失败 1：成功）
*/
bool MS4220_UART::findState(String* str, char c1, char c2){
	uint8_t startAddr,endAddr;
	startAddr = (*str).indexOf(c1);
	if(startAddr == -1) return 0;
	startAddr++;
	endAddr = (*str).indexOf(c2, startAddr);
	if(endAddr == -1) return 0;
	String s = (*str).substring(startAddr, endAddr);
	for(uint8_t i=0; i<7; i++){
		if(s == stateToStr(i)){
			status.state = i;
			return 1;
		}
	}
	status.state = -1;
	return 0;
}

/*
  @说明 解析返回状态
  @参数	str		状态字符串
  @返回 bool 	读取成功标志（0：失败 1：成功）
*/
bool MS4220_UART::dealstatus(String* str){
	String s;
	//获取状态
	if(findState(str, '<',',')==0) return 0;
	//获取位置
	if(findNum(str, "WPos", 1, &s)==0) return 0;
	status.pos = s.toInt();
	//获取速度
	if(findNum(str, "FPct", 1, &s)==0) return 0;
	status.feed = s.toInt();
	return 1;
}
