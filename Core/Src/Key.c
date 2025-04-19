#include "Key.h"

int is_mode = 0;    //控制确认
int mode_flag = 0;  //控制+-
int flag = 0;       //控制目前到第几个模式

void Key_process(){
  if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET) {
    HAL_Delay(20);
    while(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET);
    HAL_Delay(20);
    mode_flag = 1;
  }
  else if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET) {
    HAL_Delay(20);
    while (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET);
    HAL_Delay(20);
    mode_flag = -1;
  }
  else if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET) {
    HAL_Delay(20);
    while(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET);
    HAL_Delay(20);
    is_mode = 1;
  }
}