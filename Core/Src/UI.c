#include "UI.h"
#include "oled.h"
#include <stdio.h>

int flag = 0;       //控制目前到第几个模式
int key = 0;
extern char message_key[20];
extern int currentkey;

void OLED_State(){
  if(currentkey == 0){
    OLED_NewFrame();
    OLED_PrintString(20, 25, "Init Success", &font16x16, OLED_COLOR_NORMAL);
    sprintf(message_key, "currentmode: %d", currentkey);
    OLED_PrintString(20, 42, message_key, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
  }
  else if (currentkey == 1) {
    OLED_NewFrame();
    OLED_PrintString(0, 0, "mode1", &font16x16, OLED_COLOR_NORMAL);
    sprintf(message_encoder, "angle: %.2f", encoder_result);
    OLED_PrintString(0, 17, message_encoder, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
  }
  else if (currentkey == 2) {
    OLED_NewFrame();
    sprintf(message_encoder, "angle: %.2f", encoder_result);
    OLED_PrintString(0, 17, message_encoder, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
  }
}
