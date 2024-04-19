#include "Mirobot.h"

Mirobot_UART::Mirobot_UART(RS485AssicMaster *p){
	pSerial = p;
}

/*
  @说明	设置设备的通讯相关参数
  @参数	*p		串口号
  @参数	addr	设备地址，范围：1-247，0为广播地址
							如果该串口使用uart，而非RS485，可以不设置地址，或将地址设为-1
  @返回 无
*/
void Mirobot_UART::init(int addr){
	address = addr;
}

/*
  @说明	发送指令，可选是否等待应答
  @参数	str		要发送的字符串，不需要带地址，自动添加
  @参数	askEn	应答等待使能（0：不等待应答 1：等待应答）
  @返回 无
*/
void Mirobot_UART::sendMsg(String str, bool askEn){
	pSerial->send(str, address, askEn);
	sendTime = millis();
}

/*
  @说明 接收指令，可选是否等待应答
  @参数	无
  @返回 bool	接收应答标志（0：应答完成 1：等待应答）
*/
bool Mirobot_UART::receive(){
	while(pSerial->getAckState()){
		String str = pSerial->receive();

		if(str.startsWith("State")) findState(&str, ',',',');
		else if(str.startsWith("<")) dealstatus(&str);
		else if(str.startsWith("Mirobot") || str.startsWith("Dark3") || str.startsWith("E4")){
			uint8_t size=str.length();
			firmwareVer = str.substring(0,size-2);
		}
		else if(str.startsWith("EXbox")){
			uint8_t size=str.length();
			exboxVer = str.substring(0,size-2);
		}
		else if(str.endsWith("ok\r\n"));
		else;
	}
}

/*
  @说明 获取基本信息，版本号
  @参数	无
  @返回 String
*/
String Mirobot_UART::getVersions(){
	firmwareVer = "";
	exboxVer = "";
	sendMsg("$v\r\n", 1);
	receive();
	if(firmwareVer == ""){
		receive();
	}
	return firmwareVer + "\r\n" + exboxVer + "\r\n";
}

/*
  @说明 读取运动状态
  @参数	无
  @返回 int
*/
int Mirobot_UART::getState(){
	while(millis()-sendTime < STATEDELAY);//给发送预留时间
	sendMsg("O103\r\n", 1);
	receive();
	return status.state;
}

/*
  @说明 读取运动状态 字符串类型
  @参数	无
  @返回 String
*/
String Mirobot_UART::getStateToStr(){
	while(millis()-sendTime < STATEDELAY);//给发送预留时间
	int num = getState();
	if(num==-1) return "Error";
	return stateToStr(num);
}

/*
  @说明 读取所有状态
  @参数	无
  @返回 状态集
*/
STATUS_MIROBOT Mirobot_UART::getStatus(){
	while(millis()-sendTime < STATEDELAY);//给发送预留时间
	sendMsg("?\r\n", 1);
	receive();
	return status;
}

/*
  @说明 等待空闲状态
  @参数	无
  @返回 无
*/
void Mirobot_UART::waitIdle(){
	while(getState() != Idle);//等待空闲
}

/*
  @说明 归零
  @参数	mode	0/$H：设备和扩展轴一起归零；
							1/$H1：1轴归零；
							2/$H2：2轴归零；
							3/$H3：3轴归零；
							4/$H4：4轴归零；
							5/$H5：5轴归零；
							6/$H6：6轴归零（仅特定版本设备支持）；
							7/$H7：扩展轴归零（需要接传感器）；
							8/$H：设备轴一起归零；
							9/$HH：设备轴分别归零；
							10/$HE：设备轴最小姿态归零；
  @返回 无
*/
void Mirobot_UART::homing(int mode){
  switch(mode){
		case 0: sendMsg("O105=0\r\n", 1); break;//$H0
    case 1: sendMsg("O105=1\r\n", 1); break;//$H1
    case 2: sendMsg("O105=2\r\n", 1); break;//$H2
    case 3: sendMsg("O105=3\r\n", 1); break;//$H3
    case 4: sendMsg("O105=4\r\n", 1); break;//$H4
    case 5: sendMsg("O105=5\r\n", 1); break;//$H5
    case 6: sendMsg("O105=6\r\n", 1); break;//$H6
    case 7: sendMsg("O105=7\r\n", 1); break;//$H7
		case 8: sendMsg("O105\r\n", 1); break;//$H
		case 9: sendMsg("O105=9\r\n", 1); break;//$HH
		case 10: sendMsg("O105=10\r\n", 1); break;//$HE
    default: sendMsg("O105\r\n", 1); break;//$H
  }
	receive();
}

/*
  @说明 移动到零点位置
  @参数	无
  @返回 无
*/
void Mirobot_UART::zero(){    //设备初始位置
  sendMsg("M21 G90 G00 X0 Y0 Z0 A0 B0 C00\r\n", 1);//关节模式运动到零点
	receive();
}

/*
  @说明 机械臂运动
  @参数	pathMode		路径模式
						MOVEP 点到点运动
						MOVEL 直线运动
						JUMP  门型运动
	@参数	motionMode	运动模式
						ABS   绝对位置
						INC   相对位置
	@参数	x		x坐标值
	@参数	y		y坐标值
	@参数	z		z坐标值
	@参数	a		a坐标值
	@参数	b		b坐标值
	@参数	c		c坐标值
  @返回 无
*/
void Mirobot_UART::movePose(uint8_t pathMode, bool motionMode, float x, float y, float z, float a, float b, float c){
	String str = "";
	str += "M20";
	switch(motionMode){
		case ABS: str += "G90"; break;
		case INC: str += "G91"; break;
	}
	switch(pathMode){
		case MOVEP: str += "G00"; break;
		case MOVEL: str += "G01"; break;
		case JUMP : str += "G05"; break;
	}
	str += "X"; str += x;
	str += "Y"; str += y;
	str += "Z"; str += z;
	str += "A"; str += a;
	str += "B"; str += b;
	str += "C"; str += c;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 机械臂运动，含扩展轴
  @参数	pathMode		路径模式
						MOVEP 点到点运动
						MOVEL 直线运动
						JUMP  门型运动
	@参数	motionMode	运动模式
						ABS   绝对位置
						INC   相对位置
	@参数	x		x坐标值
	@参数	y		y坐标值
	@参数	z		z坐标值
	@参数	a		a坐标值
	@参数	b		b坐标值
	@参数	c		c坐标值
	@参数	d		d坐标值
  @返回 无
*/
void Mirobot_UART::movePoseWithExj(uint8_t pathMode, bool motionMode, float x, float y, float z, float a, float b, float c, float d){
	String str = "";
	str += "M20";
	switch(motionMode){
		case ABS: str += "G90"; break;
		case INC: str += "G91"; break;
	}
	switch(pathMode){
		case MOVEP: str += "G00"; break;
		case MOVEL: str += "G01"; break;
		case JUMP : str += "G05"; break;
	}
	str += "X"; str += x;
	str += "Y"; str += y;
	str += "Z"; str += z;
	str += "A"; str += a;
	str += "B"; str += b;
	str += "C"; str += c;
	str += "D"; str += d;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 机械臂圆弧插补运动
  @参数	pathMode		路径模式	
						CW 	顺时针画弧
						CCW 逆时针画弧
	@参数	motionMode	运动模式
						ABS	绝对位置
						INC	相对位置
	@参数	x		x坐标值
	@参数	y		y坐标值
	@参数	z		z坐标值
	@参数	r		圆弧半径
  @返回 无
*/
void Mirobot_UART::moveArc(bool pathMode, bool motionMode, float x, float y, float z, float r){
	String str = "";
	str += "M20";
	switch(motionMode){
		case ABS: str += "G90"; break;
		case INC: str += "G91"; break;
	}
	switch(pathMode){
		case CW : str += "G02"; break;
		case CCW: str += "G03"; break;
	}
	str += "X"; str += x;
	str += "Y"; str += y;
	str += "Z"; str += z;
	str += "R"; str += r;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 机械臂关节运动
  @参数	motionMode	运动模式
						ABS	绝对位置
						INC	相对位置
	@参数	j1		1轴角度值
	@参数	j2		2轴角度值
	@参数	j3		3轴角度值
	@参数	j4		4轴角度值
	@参数	j5		5轴角度值
	@参数	j6		6轴角度值
  @返回 无
*/
void Mirobot_UART::moveJoints(bool motionMode, float j1, float j2, float j3, float j4, float j5, float j6){
	String str = "";
	str += "M21";
	switch(motionMode){
		case ABS: str += "G90"; break;
		case INC: str += "G91"; break;
	}
	str += "G01";
	str += "X"; str += j1;
	str += "Y"; str += j2;
	str += "Z"; str += j3;
	str += "A"; str += j4;
	str += "B"; str += j5;
	str += "C"; str += j6;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*				
//点到点运动
void Mirobot_UART::moveP(float x, float y, float z, float a, float b, float c){
	move(MOVEP, ABS, x, y, z, a, b, c);
}
//直线运动
void Mirobot_UART::moveL(float x, float y, float z, float a, float b, float c){
	move(MOVEL, ABS, x, y, z, a, b, c);
}
//关节运动
void Mirobot_UART::moveJ(float x, float y, float z, float a, float b, float c){
	move(MOVEJ, ABS, x, y, z, a, b, c);
}
//点到点相对运动
void Mirobot_UART::moveIncP(float x, float y, float z, float a, float b, float c){
	move(MOVEP, INC, x, y, z, a, b, c);
}
//直线相对运动
void Mirobot_UART::moveIncL(float x, float y, float z, float a, float b, float c){
	move(MOVEL, INC, x, y, z, a, b, c);
}
//关节相对运动
void Mirobot_UART::moveIncJ(float x, float y, float z, float a, float b, float c){
	move(MOVEJ, INC, x, y, z, a, b, c);
}
//圆弧运动
void Mirobot_UART::moveC(float x, float y, float z, float r, float b, float c){
	move(MOVEP, ABS, x, y, z, a, b, c);
}
//圆弧相对运动
void Mirobot_UART::moveIncC(float x, float y, float z, float a, float b, float c){
	move(MOVEP, ABS, x, y, z, a, b, c);
}
*/

/*
  @说明 设置运动速度
	@参数	speed		速度值?
  @返回 无
*/
void Mirobot_UART::setMotionSpeed(float speed){
	String str = "";
	str += "F";
	str += speed;
	str += "\r\n";
  sendMsg(str, 1);
	receive();
}

/*
  @说明 设置运动速度百分比
	@参数	ratio		运动速度百分比 范围：1-100
  @返回 无
*/
void Mirobot_UART::motionSpeedRatio(uint8_t ratio){
	String str = "";
	str += "H";
	str += ratio;
	str += "\r\n";
  sendMsg(str, 1);
	receive();
}

/*
  @说明 暂停运动
	@参数	无
  @返回 无
*/
void Mirobot_UART::movePause(){
	sendMsg("!\r\n", 0);
	receive();
}

/*
  @说明 继续运动
	@参数	无
  @返回 无
*/
void Mirobot_UART::moveContinue(){
	sendMsg("~\r\n", 0);
	receive();
}

/*
  @说明 停止运动
	@参数	无
  @返回 无
*/
void Mirobot_UART::moveStop(){
	sendMsg("%\r\n", 0);
	receive();
}

/*
  @说明 设置扩展轴减速比
	@参数	ratio		1个单位对应的脉冲数量
  @返回 无
*/
void Mirobot_UART::setExjRatio(float ratio){
	exj_ratio = ratio;
}

/*
  @说明 扩展轴运动-单位脉冲
  @参数	motionMode	运动模式
						ABS	绝对位置
						INC	相对位置
	@参数	d		扩展轴值
  @返回 无
*/
void Mirobot_UART::moveExjPulse(bool motionMode, int32_t d){
	String str = "";
	str += "M21";
	switch(motionMode){
		case ABS: str += "G90"; break;
		case INC: str += "G91"; break;
	}
	str += "G00 D";
	str += d;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 扩展轴运动-单位距离
  @参数	motionMode	运动模式
						ABS	绝对位置
						INC	相对位置
	@参数	d		扩展轴值
  @返回 无
*/
void Mirobot_UART::moveExjDist(bool motionMode, float d){
	String str = "";
	str += "M21";
	switch(motionMode){
		case ABS: str += "G90"; break;
		case INC: str += "G91"; break;
	}
	str += "G00 D";
	str += d*exj_ratio;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 控制电动夹爪状态
  @参数	num		电动夹爪状态 （0/OFF:关闭 1/OPEN:打开 2/CLOSE:关闭）
  @返回 无
*/	
void Mirobot_UART::setEndtGripper(uint8_t num){
	String str = "";
	if(num == OFF) str = "M3 S0\r\n";
	else if(num == OPEN) str = "M3 S40\r\n";
	else if(num == CLOSE) str = "M3 S60\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 控制气泵状态
  @参数	num		pwm占空比 （0/OFF:关闭 1/IN:正压 2/OUT:负压）
  @返回 无
*/	
void Mirobot_UART::setEndtPump(uint8_t num){
	String str = "";
	if(num == OFF) str = "M3 S0\r\n";
	else if(num == OUT) str = "M3 S500\r\n";
	else if(num == IN) str = "M3 S1000\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 pwm输出，控制器黄色接口
  @参数	num		pwm占空比 范围：0~1000
  @返回 无
*/	
void Mirobot_UART::setEndtPwm(uint16_t num){
	sendMsg("M3 S"+String(num)+"\r\n", 1);
	receive();
}

/*
  @说明 运行文件名
  @参数	fileName	文件名
	@参数	loop			循环标志 （false:不循环 true:循环）
  @返回 无
*/	
void Mirobot_UART::runFile(String fileName, bool loop){
	String str = "";
	if(loop) str += "O112=";
	else str += "O111=";
	str += fileName;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 运行文件号
  @参数	fileNum		文件号，取文件名称前两位数字
	@参数	loop			循环标志 （false:不循环 true:循环）
  @返回 无
*/
void Mirobot_UART::runFileNum(uint8_t fileNum, bool loop){
	String str = "";
	if(loop) str += "O116=";
	else str += "O116=";
	str += fileNum;
	str += "\r\n";
	sendMsg(str, 1);
	receive();
}

/*
  @说明 设备重启
  @参数	无
  @返回 无
*/
void Mirobot_UART::reset(){
	sendMsg("O100\r\n", 1);
	receive();
}

/*
  @说明 查找指定字符串后的数据
  @参数	str			状态所在的字符串
	@参数	fStr		要查找的字符串
	@参数	num			寻找字符串后的第几个数据,最小为1
	@参数	retStr	读取到的数据
  @返回 bool 		读取成功标志（0：失败 1：成功）
*/
bool Mirobot_UART::findNum(String* str, String fStr, uint8_t num, String* retStr){
	uint8_t startAddr, endAddr;
	startAddr = (*str).indexOf(fStr);
	if(startAddr == -1) return 0;
	endAddr = (*str).indexOf(':', startAddr);
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
bool Mirobot_UART::findState(String* str, char c1, char c2){
	uint8_t startAddr, endAddr;
	status.state = -1;
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
	return 0;
}

/*
  @说明 解析返回状态
  @参数	str		状态字符串
  @返回 bool 	读取成功标志（0：失败 1：成功）
*/
bool Mirobot_UART::dealstatus(String* str){
	String s;
	uint8_t i;
	//获取状态
	if(findState(str, '<',',')==0) return 0;
	//获取角度
	for(i=0; i<7; i++){
		if(findNum(str, "Angle", i+1, &s)==0) return 0;
		status.angle[(i+3)%7] = s.toFloat();
	}
	//获取位置
	for(i=0; i<6; i++){
		if(findNum(str, "Cartesian", i+1, &s)==0) return 0;
		status.cartesian[i] = s.toFloat();
	}
	//获取pwm
	if(findNum(str, "Pump", 1, &s)==0) return 0;
	status.pumpPwm = s.toInt();
	return 1;	
}