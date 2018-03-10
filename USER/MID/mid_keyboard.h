#ifndef __MID_KEYBOARD_H
#define __MID_KEYBOARD_H

#include "hal_keyboard.h"
#include "hal_timer.h"

#define START_KEY_TIMEOUT 		3
#define CLEAR_KEY_TIMEOUT		3	



void MID_KEYBOARD_TASK(void);
void MID_KEYBOARD_KeyPressInit(void);

#endif

