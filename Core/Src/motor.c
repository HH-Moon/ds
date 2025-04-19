#include "motor.h"

#define PWM_MAX 7200
#define PWM_MIN -7200

extern TIM_HandleTypeDef htim4;
int dead_area = 1000;

int abs(int n)
{
    if(n >= 0)
    {
        return n;
    }
    else
    {
        return -n;
    }
}

void Load(int moto1)   //-7200~7200
{
    if(moto1 < 0)   //顺时针 编码器从65535开始
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);     //AIN1 = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //AIN2 = 1;
    }
    else            //逆时针 编码器从0开始
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);      //AIN1 = 1;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);    //AIN2 = 0;
    }
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, abs(moto1) + dead_area);
}

void Limit(int *Moto1){
    if(*Moto1 > PWM_MAX) *Moto1 = PWM_MAX;
    if(*Moto1 < PWM_MIN) *Moto1 = PWM_MIN;
}
