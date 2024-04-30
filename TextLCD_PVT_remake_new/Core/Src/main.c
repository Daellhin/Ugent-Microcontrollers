/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stdio.h"
#include "string.h"
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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int infrared_state;
uint32_t infrared_code;

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {  //list found on github
		// XGFEDCBA
				0b00111111,// 0
				0b00000110,    // 1
				0b01011011,    // 2
				0b01001111,    // 3
				0b01100110,    // 4
				0b01101101,    // 5
				0b01111101,    // 6
				0b00000111,    // 7
				0b01111111,    // 8
				0b01101111,    // 9
				0b01110111,    // A
				0b01111100,    // b
				0b00111001,    // C
				0b01011110,    // d
				0b01111001,    // E
				0b01110001     // F
		};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Display_CLK(uint8_t level) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, level);
	//HAL_Delay(1);
}

void Display_DIO(uint8_t level) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, level);
	//HAL_Delay(1);
}

void I2Cstart(void) {
	Display_DIO(1);
	Display_CLK(1); //I2C start condition
	Display_DIO(0); //high to low data while clk high
	Display_CLK(0);
}

void I2Cstop(void) {
	Display_DIO(0);
	Display_CLK(1); //I2C stop condition
	Display_DIO(1); //low to high data while clk high

}

void I2Cwritebyte(uint8_t databyte)  //LSB first for TM1637 chip!!!
{
	for (uint8_t n = 0; n <= 7; n++) {
		if ((databyte & 0x01) > 0)
			Display_DIO(1);
		else
			Display_DIO(0);
		Display_CLK(1);
		databyte >>= 1;
		Display_CLK(0);
	}
	Display_CLK(1); //9th clk  ACK ignored
	Display_CLK(0);
}

void I2Cwritesegment(uint8_t segmentnr, uint8_t hexdigit) {
	I2Cstart();
	I2Cwritebyte(0x40);  //command1 write data to display register
	I2Cstop();
	I2Cstart();
	I2Cwritebyte(0xc0 + (segmentnr & 0x03));  //command2  display address
	I2Cwritebyte(digitToSegment[hexdigit & 0x0F]);  //data1
	I2Cstop();

	I2Cstart();
	I2Cwritebyte(0x88);  //display control --> display on
	I2Cstop();
}

void I2CwriteDECnumber(unsigned int number) {
	for (int8_t digitnr = 3; digitnr >= 1; digitnr--) {
		I2Cwritesegment(digitnr, number % 10);
		number /= 10;
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

	//codes for white remote control
	uint32_t white_raw_codes[10] = { 0xff6897, 0xff30cf, 0xff18e7, 0xff7a85,
			0xff10ef, 0xff38c7, 0xff5aa5, 0xff42bd, 0xff4ab5, 0xff52ad }; //numbers from 0 to 9

	//codes for black remote control
	uint32_t black_raw_codes[10] = { 0xff9867, 0xffa25d, 0xff629d, 0xffe21d,
			0xff22dd, 0xff02fd, 0xffc23d, 0xffe01f, 0xffa857, 0xff906f }; //numbers from 0 to 9

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	infrared_state = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	unsigned int getal = 0;
	char text[20];

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0); //put red LED off

	// DEMO of the infrared decoding state machine via an external interrupt
	// pvantorre 09/04/2021

	while (1) {
		infrared_state = 0; //put infrared state machine in ISR in standby for new code
		while (infrared_state < 5) //wait until valid code is fully received
		{
			getal++; // in the meantime let a counter run full speed on the LED displays
			if (getal > 999) //to demonstrate you don't waste processing time waiting for the infrared sensor
				getal = 0;

			I2CwriteDECnumber(getal);
		}

		//if infrared_state == 5 a code was received
		//the raw 32-bits code is in infrared_code
		//bits from MSB to LSB are in the order as received

		//now look up if the raw code corresponds to a digit from 0 to 9 on the *** WHITE remote ***
		int digit = -1;
		for (int n = 0; n <= 9; n++)
			if (infrared_code == white_raw_codes[n]) {
				digit = n;

				sprintf(text, "White: %d \r\n", digit);
				HAL_UART_Transmit(&huart1, (uint8_t*) text, strlen(text), 100);

				I2Cwritesegment(0, digit); //display the digit that was sent in the first display position
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0); //put red LED off --> white remote used for number entry
				break;
			}
		//so here digit is 0-9 if number pressed
		//else digit == -1 invalid
		//extra feature to see the counter value
		if (infrared_code == 0xffb04f) //check raw code for C button on white remote
			HAL_Delay(1000); //stop counter on display for 1 second

//*********************************************ONLY NECESSARY FOR BLACK REMOTE  **********************************START
		if (digit == -1) { //maybe it's a code from the black remote control????
						   //now look up if the raw code corresponds to a digit from 0 to 9 on the *** BLACK remote ***
			int digit = -1;
			for (int n = 0; n <= 9; n++)
				if (infrared_code == black_raw_codes[n]) {
					digit = n;

					sprintf(text, "Black: %d \r\n", digit);
					HAL_UART_Transmit(&huart1, (uint8_t*) text, strlen(text),
							100);

					I2Cwritesegment(0, digit); //display the digit that was sent in the first display position
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1); //put red LED on --> black remote used for number entry
					break;
				}
		}

		//extra feature to see the counter value
		if (infrared_code == 0xffb04f) //check raw code for # button on black remote
			HAL_Delay(1000); //stop counter on display for 1 second

//*********************************************ONLY NECESSARY FOR BLACK REMOTE  **********************************END

	}

	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 80;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : INFRARED_Pin */
  GPIO_InitStruct.Pin = INFRARED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INFRARED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
