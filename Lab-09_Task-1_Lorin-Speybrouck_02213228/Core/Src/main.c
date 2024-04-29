/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include <stdbool.h>
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
I2C_HandleTypeDef hi2c1;

LPTIM_HandleTypeDef hlptim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
// Numbers(white-remote) 0 to 9
const uint32_t white_number_codes[10] = { 0xff6897, 0xff30cf, 0xff18e7,
		0xff7a85, 0xff10ef, 0xff38c7, 0xff5aa5, 0xff42bd, 0xff4ab5, 0xff52ad };

// Numbers(black-remote) 0 to 9
const uint32_t black_number_codes[10] = { 0xff9867, 0xffa25d, 0xff629d,
		0xffe21d, 0xff22dd, 0xff02fd, 0xffc23d, 0xffe01f, 0xffa857, 0xff906f };

const uint32_t clear_code = 0xffb04f;
const uint32_t add_code = 0xff02fd;
const uint32_t subtract_code = 0;
const uint32_t play_code = 0;

int infrared_state;
uint32_t infrared_code;

const uint8_t digitToSegment[] = {
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
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPTIM1_Init(void);
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
	for (int8_t digitnr = 3; digitnr >= 0; digitnr--) {
		I2Cwritesegment(digitnr, number % 10);
		number /= 10;
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_LPTIM1_Init();
	/* USER CODE BEGIN 2 */

	// -- Reset outputs --
	HAL_GPIO_WritePin(GPIOA, RED_LED_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, 0);
	I2CwriteDECnumber(0);

	char text[20];
	sprintf(text, "Hi: d\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*) text, strlen(text), 100);

	int number1 = 0; // first number or answer
	int number2 = 0;
	int addOrSubtract = 0; // 0=add, 1=subtract
	int calculationSet = 0;
	int calculated = 0;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// -- Reset IR state machine --
		infrared_state = 0;
		// -- Wait for IR code --
		while (infrared_state < 5) {
		}

		// -- Check received code --
		if (infrared_code == clear_code) {
			number1 = 0;
			number2 = 0;
			addOrSubtract = 0;
			calculationSet = 0;
			calculated = 0;
			I2CwriteDECnumber(number1);
			HAL_GPIO_WritePin(GPIOA, RED_LED_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, 0);
			break;
		}
		if (infrared_code == play_code) {
			if (addOrSubtract == 0) {
				number1 = number1 + number2;
			} else {
				number1 = number1 - number2;
			}
			number2 = 0;
			calculated = 1;
			I2CwriteDECnumber(number1);
			HAL_GPIO_WritePin(GPIOA, RED_LED_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, 0);
			break;
		}
		if (infrared_code == add_code) {
			addOrSubtract = 0;
			calculationSet = 1;
			HAL_GPIO_WritePin(GPIOA, RED_LED_Pin, 1);
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, 0);
			break;
		}
		if (infrared_code == subtract_code) {
			addOrSubtract = 1;
			calculationSet = 1;
			HAL_GPIO_WritePin(GPIOA, RED_LED_Pin, 0);
			HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, 1);
			break;
		}
		for (int n = 0; n <= 9; n++) {
			if (infrared_code == white_number_codes[n]) {
				if (calculationSet == 0) {
					// If answer is stil shown, but no calculation has been set, reset answer
					if (calculated) {
						number1 = 0;
						calculated = 0;
					}
					number1 = number1 * 10 + n;
					I2CwriteDECnumber(number1);
				} else {
					number2 = number2 * 10 + n;
					I2CwriteDECnumber(number2);
				}
				break;
			}
		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}

	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x10909CEC;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief LPTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPTIM1_Init(void) {

	/* USER CODE BEGIN LPTIM1_Init 0 */

	/* USER CODE END LPTIM1_Init 0 */

	/* USER CODE BEGIN LPTIM1_Init 1 */

	/* USER CODE END LPTIM1_Init 1 */
	hlptim1.Instance = LPTIM1;
	hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
	hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
	hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
	hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
	hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
	hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
	hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
	hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
	if (HAL_LPTIM_Init(&hlptim1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN LPTIM1_Init 2 */

	/* USER CODE END LPTIM1_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

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
	if (HAL_UART_Init(&huart1) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2 | RED_LED_Pin | GPIO_PIN_7,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA2 RED_LED_Pin PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | RED_LED_Pin | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PD14 */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : GREEN_LED_Pin */
	GPIO_InitStruct.Pin = GREEN_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GREEN_LED_GPIO_Port, &GPIO_InitStruct);

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
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
