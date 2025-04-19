#include "UI.h"
#include "Key.h"
#include "oled.h"
#include <stdio.h>

void OLED_State(){
  flag += mode_flag;
  OLED_NewFrame();
  if(flag == 0){
    OLED_PrintString(20, 25, "Init Success", &font16x16, OLED_COLOR_NORMAL);
  }
  else if (flag == 1) {
    OLED_PrintString(0, 0, "mode1", &font16x16, OLED_COLOR_NORMAL);
    sprintf(message_encoder, "angle: %.2f", Encoder_Angle);
    OLED_PrintString(0, 17, message_encoder, &font16x16, OLED_COLOR_NORMAL);
  }
  OLED_ShowFrame();
}