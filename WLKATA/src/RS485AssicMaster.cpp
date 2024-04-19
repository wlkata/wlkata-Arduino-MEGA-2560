#include "RS485AssicMaster.h"

#ifdef RS485_SERIAL_PORT
RS485AssicMaster RS485(RS485_SERIAL_PORT);
#else
RS485AssicMaster RS485(&Serial1);
#endif

RS485AssicMaster::RS485AssicMaster(HardwareSerial* hwSerial){
	pSerial = hwSerial;
}

/*
  @说明	设置设备的通讯相关参数
  @参数	*p		串口号
  @参数	addr	设备地址，范围：1-247，0为广播地址
							如果该串口使用uart，而非RS485，可以不设置地址，或将地址设为-1
  @返回 无
*/
void RS485AssicMaster::begin(unsigned long baud){
	baudrate = baud;
	pSerial->begin(baudrate);
	pSerial->setTimeout(ReceiveDelay);
	//pSerial->println("Debug,test");
}

void RS485AssicMaster::begin(HardwareSerial* hwSerial, unsigned long baud){
	pSerial = hwSerial;
	begin(baud);
}

void RS485AssicMaster::beginTransmission(int addr, bool waitAckEN){
	while(pSerial->available()) pSerial->read();//清空缓存
	address = addr;
	if(addr != -1){
		sendStr = "@" + String(addr);//生成开头字符
		pSerial->print(sendStr);//发送
	}
	else sendStr = "";
	
	//应答信号
	if(addr != 0 && waitAckEN) ackState = 1;
	else ackState = 0;
}

void RS485AssicMaster::writeData(String str){
	pSerial->print(str);//发送
	sendStr += str;
}

void RS485AssicMaster::endTransmission(){
	sendTime = millis();//记录发送时间
	if(monitorFunctionFlag) (*monitorFunc)(sendStr, 1);//发送 发到监视回调函数
}

void RS485AssicMaster::send(String str, int addr, bool waitAckEN){
	beginTransmission(addr, waitAckEN);
	writeData(str);
	endTransmission();
}

uint8_t RS485AssicMaster::getAckState(){
	pSerial->flush();//等待发送结束
	if(ackState == 1){//待应答
		//Serial.println("Debug,wait ack");
		while(1){
			if(pSerial->available()){
				receiveStr = pSerial->readStringUntil('\n');//读取
				//Serial.println("Debug,cmd:"+receiveStr);
				//Serial.println("Debug,cmd length:"+String(receiveStr.length()));
				if(receiveStr.endsWith("\r")){//有格式完整的返回
					receiveStr += '\n';
					ackState = 2;
					if(monitorFunctionFlag) (*monitorFunc)(receiveStr, 0);//接收 发到监视回调函数
				}
				else{//有格式不完整的返回
					receiveStr = "";
					ackState = 0;
				}
				break;
			}
			else if(millis()-sendTime > OUTTIME){//无应答 或 应答超时
				ackState = 0;
				if(noAskFunctionFlag) (*outTimeFunc)(sendStr, address);
				break;
			}
		}
	}
	else if(ackState == 3){
		receiveStr = pSerial->readStringUntil('\n');//读取
		if(receiveStr.endsWith("\r")){
			receiveStr += '\n';
			ackState = 2;
			if(monitorFunctionFlag) (*monitorFunc)(receiveStr, 0);//接收 发到监视回调函数
		}
		else{
			receiveStr = "";
			ackState = 0;
		}
	}
	return ackState;
}

String RS485AssicMaster::receive(){
	String ret = "";
	if(getAckState() == 2){
		ret = receiveStr;
		if(getAckState()) ackState = 3;
		else ackState = 0;
	}
	return ret;
}

void RS485AssicMaster::setOutTimeFunction(void (*_func)(String, int), uint32_t time){
	noAskFunctionFlag = true;
	outTimeFunc = _func;
	ackMaxTime = time;
}

void RS485AssicMaster::setOutTime(uint32_t time){
	ackMaxTime = time;
}

void RS485AssicMaster::setMonitorFunction(void (*_func)(String, bool)){
	monitorFunctionFlag = true;
	monitorFunc = _func;
}
