#include "UI.h"
#include "oled.h"
#include <stdio.h>

int is_mode = 0;    //控制确认
int mode_flag = 0;  //控制+-
int flag = 0;       //控制目前到第几个模式
extern int KEY;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case KEY1_Pin:
      KEY = 1;
    break;
    case KEY2_Pin:
      KEY = 2;
    break;
    case KEY3_Pin:
      KEY = 3;
    break;
    case KEY4_Pin:
      KEY = 4;
    break;
    case KEY5_Pin:
      KEY = 5;
    break;
    default:
      break;
  }
}

void OLED_State(){
  if(KEY == 0){
    OLED_PrintString(20, 25, "Init Success", &font16x16, OLED_COLOR_NORMAL);
  }
  else if (KEY == 1) {
    OLED_PrintString(0, 0, "mode1", &font16x16, OLED_COLOR_NORMAL);
    sprintf(message_encoder, "angle: %.2f", encoder_result);
    OLED_PrintString(0, 17, message_encoder, &font16x16, OLED_COLOR_NORMAL);
  }
}