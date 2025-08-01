#pragma once
#include "SDT.h"

#define CW_SHAPING_NONE 0
#define CW_SHAPING_RISE 1
#define CW_SHAPING_FALL 2
#define CW_SHAPING_ZERO 3

extern uint8_t keyPressedOn;

void KeyTipOn();
void KeyRingOn();
void CW_ExciterIQData(int shaping);
