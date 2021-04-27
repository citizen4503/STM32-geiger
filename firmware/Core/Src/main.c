/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nokia5110_LCD.h"
#include <stdio.h>
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
I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/* sts5 */
uint32_t sts5_counter = 0;
uint32_t sts5_counter_minute = 0;
uint32_t sts5_counter_second = 0;
/* rtc */
RTC_TimeTypeDef RtcTime;
uint16_t CompareSeconds;
/* array for output */
char Message[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SetRTC(void);
void ClearCounter(void);
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
  MX_I2C2_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* Initialize LCD pins */
  LCD_setRST(GPIOA, LCD_RST_Pin);
  LCD_setCE(GPIOA, LCD_CE_Pin);
  LCD_setDC(GPIOA, LCD_DC_Pin);
  LCD_setDIN(GPIOA, LCD_DIN_Pin);
  LCD_setCLK(GPIOA, LCD_CLK_Pin);
  /* Init Display */
  LCD_init();
  // Start timer
  HAL_TIM_Base_Start(&htim3);

  // flags for menu interface
  uint8_t select = 0;
  uint8_t select_item = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* USER CODE BEGIN 3 */
	  switch(select)
	  {
	  case 0:
		  /******************************************************************/
		  /* 					DASHBOARD SECTION BEGIN						*/
		  /******************************************************************/
		  while(1){
			  // Get RTC time
			  HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
			  sprintf(Message, "TIME: %02d:%02d:%02d", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
			  LCD_print(Message, 0, 0);

			  // total count
			  if(sts5_counter < 99999999){
				  sprintf(Message, "CT  : %lu        ", sts5_counter);
				  LCD_print(Message, 0, 1);
			  }else if(sts5_counter >= 99999999){
				  // reach maximum value for total counter
				  sprintf(Message, "CT  :    ERROR");
				  LCD_print(Message, 0, 1);
			  }

			  // count per minute
			  if(sts5_counter_minute < 99999999){
				  sprintf(Message, "CPM : %lu        ", sts5_counter_minute*60);
				  LCD_print(Message, 0, 2);
			  }else if(sts5_counter_minute >= 99999999){
				  // reach maximum value for count per minute
				  sprintf(Message, "CPM :    ERROR");
				  LCD_print(Message, 0, 2);
			  }

			  // count per second
			  if(sts5_counter_second < 99999999){
				  sprintf(Message, "CPS : %lu        ", sts5_counter_second);
				  LCD_print(Message, 0, 3);
			  }else if(sts5_counter_second >= 99999999){
				  // reach maximum value for count per second
				  sprintf(Message, "CPS :    ERROR");
				  LCD_print(Message, 0, 3);
			  }

			  // 1 second clearing variables
			  if(RtcTime.Seconds != CompareSeconds)
			  {
				  CompareSeconds = RtcTime.Seconds;
				  // count per second reset after 1 seconds
				  sts5_counter_second = 0;
				  sts5_counter_minute = 0;
			  }

			  // hr
			  sprintf(Message, "--------------");
			  LCD_print(Message, 0, 4);

			  // hint for buttons
			  sprintf(Message, " MENU         ");
			  LCD_print(Message, 0, 5);

			  // go to menu section from dashboard
			  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, BTN1_Pin))
			  {
				  while(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, BTN1_Pin))
				  {}
				  select = 1;
				  break;
			  }
		  }
		  /******************************************************************/
		  /* 					DASHBOARD SECTION END						*/
		  /******************************************************************/
		  break;

	  case 1:
		  /******************************************************************/
		  /* 					MENU SECTION BEGIN							*/
		  /******************************************************************/
		  LCD_clrScr();
		  while(1){
			  if(select_item == 0){
				  sprintf(Message, "Time reset");
				  LCD_invertText(1);
				  LCD_print(Message, 0, 0);
				  LCD_invertText(0);
			  }else if(select_item != 0){
				  sprintf(Message, "Time reset");
				  LCD_print(Message, 0, 0);
			  }

			  if(select_item == 1){
				  LCD_invertText(1);
				  sprintf(Message, "Count reset");
				  LCD_print(Message, 0, 1);
				  LCD_invertText(0);
			  }else if(select_item != 1){
				  sprintf(Message, "Count reset");
				  LCD_print(Message, 0, 1);
			  }

			  if(select_item == 2){
				  LCD_invertText(1);
				  sprintf(Message, "All reset");
				  LCD_print(Message, 0, 2);
				  LCD_invertText(0);
			  }else if(select_item != 2){
				  sprintf(Message, "All reset");
				  LCD_print(Message, 0, 2);
			  }

			  if(select_item == 3){
				  LCD_invertText(1);
				  sprintf(Message, "Back");
				  LCD_print(Message, 0, 3);
				  LCD_invertText(0);
			  }else if(select_item != 3){
				  sprintf(Message, "Back");
				  LCD_print(Message, 0, 3);
			  }

			  // hr
			  sprintf(Message, "--------------");
			  LCD_print(Message, 0, 4);

			  // hint for buttons
			  sprintf(Message, " SELECT    OK ");
			  LCD_print(Message, 0, 5);

			  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, BTN1_Pin))
			  {
				  while(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, BTN1_Pin))
				  {}
				  if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, BTN1_Pin)){
					  select_item++;
					  if(select_item > 3){
						  select_item = 0;
					  }
				  }
			  }

			  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, BTN2_Pin))
			  {
				  while(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, BTN2_Pin))
				  {}
				  if(select_item == 0){
					  SetRTC();
				  }else if(select_item == 1){
					  ClearCounter();
				  }else if(select_item == 2){
					  SetRTC();
					  ClearCounter();
				  }else if(select_item == 3){
					  // back to main, do nothing
				  }
				  select = 0;
			  	  select_item = 0;
			  	  LCD_clrScr();
				  break;
			  }
		  }
		  /******************************************************************/
		  /* 					MENU SECTION END							*/
		  /******************************************************************/
		  break;
	  } // end switch
	  /* USER CODE END 3 */
  }
    /* USER CODE END WHILE */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 80 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin|LCD_CE_Pin|LCD_DC_Pin|LCD_DIN_Pin
                          |LCD_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_LIGHT_GPIO_Port, LCD_LIGHT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN1_Pin BTN2_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin|BTN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : STS5_Pin */
  GPIO_InitStruct.Pin = STS5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STS5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LCD_CE_Pin LCD_DC_Pin LCD_DIN_Pin
                           LCD_CLK_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_CE_Pin|LCD_DC_Pin|LCD_DIN_Pin
                          |LCD_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_LIGHT_Pin */
  GPIO_InitStruct.Pin = LCD_LIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_LIGHT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void SetRTC(void)
{
	RTC_TimeTypeDef sTime = {0};

	/** Initialize RTC and set the Time
	*/
	sTime.Hours = 00;
	sTime.Minutes = 00;
	sTime.Seconds = 00;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
	  Error_Handler();
	}
}

void ClearCounter(void){
	sts5_counter = 0;
	sts5_counter_minute = 0;
	sts5_counter_second = 0;
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
