#ifndef BASE_H
#define BASE_H

#include "Arduino.h"

#define OFF 0
#define ON 1
#define IN 1
#define OUT 2
#define OPEN 1
#define CLOSE 2

#define Offline 0
#define Idle 1
#define Alarm 2
#define Home 3
#define Run 4
#define Hold 5
#define Busy 6

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3
#define A_AXIS 4
#define B_AXIS 5
#define C_AXIS 6
#define AxisEX 7
#define AxisALL 0

String stateToStr(int num);

#endif
