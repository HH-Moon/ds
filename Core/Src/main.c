/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "font.h"
#include <stdio.h>
#include "motor.h"
#include "encoder.h"
#include <string.h>
#include "UI.h"
#include "Key.h"
#include "PID.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern PID_InitTypeDef Turn_PID1;
extern PID_InitTypeDef Turn_PID2;
extern float Transform_adc(float Value);

//编码
char message_encoder[20] = "";
int Encoder_Cnt = 0;
float Encoder_Angle = 0;
int Encoder_Integral = 0;
float encoder_result = 0;

//角度传感
char message_adc[20] = "";
uint32_t adc_value = 0;
float adc_result = 0;
uint16_t adc[20] = {0};
//UI
extern int is_mode;
extern int mode_flag;
extern int flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t ADC_Read(void)
{
  int sum = 0, max = 0, min = 4095,adc_result = 0;
  uint8_t i=0;
  HAL_ADC_Start(&hadc1);     //启动ADC转换
  HAL_ADC_PollForConversion(&hadc1, 12);
  for(i=1;i<=5;i++)
  {
    adc[i] = HAL_ADC_GetValue(&hadc1);
    sum += adc[i];
    if(adc[i] > max) max = adc[i];
    if(adc[i] < min) min = adc[i];
  }
  adc_result = (sum - max - min) / 3.0;
  return adc_result;
}

float Transform_adc(float Value) {
  float result = 0;
  if (Value > 0 && Value < 2050) {
    result = -Value / 4095 * 360;
  }
  else if (Value >2050 && Value < 4095) {
    result = 360 - Value / 4095 * 360 ;
  }
  // else if (Value == 0) {
  //   result = 0;
  // }
  // else if (Value == 2050) {
  //   result = 180;
  // }
  // else if (Value == 3090) {
  //   result = 90;
  // }
  // else if (Value == 1040) {
  //   result = -90;
  // }
  return result;
}

float Transform_encoder(float Encoder_Angle) {
  float result = 0;
  if (Encoder_Angle > 180 && Encoder_Angle < 360) {
    result = Encoder_Angle - 360;
  }
  else if (Encoder_Angle < -180 && Encoder_Angle > -360) {
    result = Encoder_Angle + 360;
  }
  return result;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim2) {
    adc_value = ADC_Read();
    Encoder_Cnt = Read_Speed(&htim3);

    Encoder_Integral += Encoder_Cnt;
    Encoder_Angle = (float)Encoder_Integral  / 1.5;
    if (Encoder_Integral > 540 || Encoder_Integral < -540) {
      Encoder_Integral = Encoder_Integral % 540;
    }
    adc_result = Transform_adc(adc_value);
    encoder_result = Transform_encoder(Encoder_Angle);
    // HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin);
    // if (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET) {
    //   Load(0);
    // }
    // else {
    //   Load(2000);
    // }
    // PID_Calculate(&Turn_PID1, 2050 - Encoder_Angle, adc_value);
    // PID_Calculate(&Turn_PID2, Encoder_Cnt+Turn_PID1.PID_Out, 0);
    // Load(Turn_PID2.PID_Out);
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start(&hadc1);     //启动ADC转换
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  // PID_Init(&Turn_PID1, -0.3, 0, -0.5, 0, 1000); // -0.2    -0.5
  // PID_Init(&Turn_PID2, -1200, 0, -150, 0, 7200);  //-1200    -150
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    OLED_NewFrame();
    sprintf(message_adc, "adc: %.2f", adc_result);
    OLED_PrintString(0, 0, message_adc, &font16x16, OLED_COLOR_NORMAL);
    sprintf(message_encoder, "angle: %.2f", encoder_result);
    OLED_PrintString(0, 33, message_encoder, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    // Key_process();
    // OLED_State();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
