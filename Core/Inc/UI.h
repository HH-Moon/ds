#ifndef __UI_H__
#define __UI_H__

#include "stm32f1xx_hal.h"

extern int is_mode;
extern int mode_flag;
extern int flag;
extern char message_encoder[20];
extern float Encoder_Angle;

void OLED_State();

#endif