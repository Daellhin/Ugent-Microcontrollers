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
LPTIM_HandleTypeDef hlptim2;

/* USER CODE BEGIN PV */
int run = 1;

//put this in the declaration section of main.c
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
// bits are .GFEDCBA
const uint8_t digitToSegment[] = { 0b00111111,    // 0
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
static void MX_LPTIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//put this in the code section of main.c but before void main(void) {}
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
	MX_LPTIM2_Init();
	/* USER CODE BEGIN 2 */
	HAL_LPTIM_TimeOut_Start_IT(&hlptim2, 32767, 32767);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		// Disable SysTick, otherwise it would wake-up the CPU within 1 ms
		HAL_SuspendTick();
		HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// K2=stop
		if (HAL_GPIO_ReadPin(GPIOA, K2_Pin) == 0) {
			run = 0;
		}
		// K1=continue
		else if (HAL_GPIO_ReadPin(GPIOB, K1_Pin) == 0) {
			run = 1;
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
 * @brief LPTIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPTIM2_Init(void) {

	/* USER CODE BEGIN LPTIM2_Init 0 */

	/* USER CODE END LPTIM2_Init 0 */

	/* USER CODE BEGIN LPTIM2_Init 1 */

	/* USER CODE END LPTIM2_Init 1 */
	hlptim2.Instance = LPTIM2;
	hlptim2.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
	hlptim2.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
	hlptim2.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
	hlptim2.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
	hlptim2.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
	hlptim2.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
	hlptim2.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
	hlptim2.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
	if (HAL_LPTIM_Init(&hlptim2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN LPTIM2_Init 2 */

	/* USER CODE END LPTIM2_Init 2 */

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
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2 | LED1_Pin | LD1_Pin | GPIO_PIN_7,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PA2 LED1_Pin LD1_Pin PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | LED1_Pin | LD1_Pin | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : K1_Pin */
	GPIO_InitStruct.Pin = K1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(K1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : K2_Pin */
	GPIO_InitStruct.Pin = K2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(K2_GPIO_Port, &GPIO_InitStruct);

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
