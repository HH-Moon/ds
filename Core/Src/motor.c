#include "motor.h"

#define PWM_MAX 7200
#define PWM_MIN -7200

extern TIM_HandleTypeDef htim4;

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
    if(moto1 < 0)   //逆时针
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);     //AIN1 = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);       //AIN2 = 1;
    }
    else            //顺时针
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);      //AIN1 = 1;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);    //AIN2 = 0;
    }
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, abs(moto1));
}

void Limit(int *moto1){
    if(*moto1 > PWM_MAX) *moto1 = PWM_MAX;
    if(*moto1 < PWM_MIN) *moto1 = PWM_MIN;
}
