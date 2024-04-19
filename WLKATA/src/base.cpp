#include "base.h"

const String stateStr[7]={"Offline", "Idle", "Alarm", "Home", "Run", "Hold", "Busy"};

String stateToStr(int num){
	return stateStr[num];
}

/*
int32_t strToInt(String str){	//读一个数字
	int8_t flag = 1;//判断正负号的标志
	int32_t num = 0;//临时存储计算结果
	
	if(str[startAddr] == '-'){//减号
		flag = -1;
		startAddr++;
	}
	
	for(startAddr; startAddr<endAddr; startAddr++){//循环直到字符串结尾
		if(str[startAddr]>='0' && str[startAddr]<='9') num = num*10+(str[startAddr]-'0');
		
		else break;
	}
	return num*flag;
}

float strToFloat(String str){	//字符串转小数
	uint8_t j = 1;
	int8_t flag = 1;//判断正负号的标志
	uint8_t flag_dot = 1;//判断小数点的标志
	float num = 0;//临时存储计算结果
	
	if(str[startAddr] == '-'){//减号
		flag = -1;
		startAddr++;
	}
	
	for(startAddr; startAddr<endAddr; startAddr++){//循环直到字符串结尾
		if(str[startAddr] == ' ');
		else if(str[startAddr]>='0' && str[startAddr]<='9'){//数字
			if(flag_dot==1){//没有小数点
				num = num*10+(str[startAddr]-'0');
			}
			else{//有小数点
				num = num+(float)(str[startAddr]-'0')*pow(0.1,j++);//运算并存储中间计算结果
			}
		}
		else if(str[startAddr] == '.') flag_dot=0;//小数点
		else break;
	}
	return num*flag;
}
*/

