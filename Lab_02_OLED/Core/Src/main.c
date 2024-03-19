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

#include "fonts.h"
#include "ssd1306.h"
#include "test.h"
#include "bitmap.h"
#include "horse_anim.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//-----------------------------------------------------------------------------
// Low-level software I2C communication to PCB unit on serial LCD display
// pvantorre FEB 2021
//
// Connect Open Smart Rich Shield UART pins top right to LCD unit
// GND----GND
// VCC----VCC
//  TX----SCL (can be remembered as TX is always an output and SCL is always an input)
//  RX----SDA
//-----------------------------------------------------------------------------
//
extern void LCD_Delay(int time); //just a predeclaration

void PCF_SCL(uint8_t level) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, level); //TX = SCL
	//HAL_Delay(1);
	LCD_Delay(10);
}

void PCF_SDA(uint8_t level) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, level); // RX = SDA
	//HAL_Delay(1);
	LCD_Delay(10);
}

uint8_t PCF_ACK(void) {
	return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1));
}

extern void I2Cstart(void) {
	PCF_SDA(1);
	PCF_SCL(1); //I2C start condition
	PCF_SDA(0); //high to low data while clk high
	PCF_SCL(0);
}

extern void I2Cstop(void) {
	//PCF_SDA(0);
	PCF_SCL(1); //I2C stop condition
	PCF_SDA(1); //low to high data while clk high

}

extern uint8_t I2Cwritebyte(uint8_t databyte)  //MSB first for PCF chip!!!
{
	uint8_t ack;
	for (uint8_t n = 0; n <= 7; n++) {
		if ((databyte & 0x80) > 0)
			PCF_SDA(1);
		else
			PCF_SDA(0);
		PCF_SCL(1);
		databyte <<= 1;
		PCF_SCL(0);
		PCF_SDA(1); //zodat ACK kan gelezen worden
	}
	PCF_SCL(1); //9th clk  ACK
	ack = PCF_ACK();
	PCF_SCL(0);
	return (ack);
}

//-----------------------------------------------------------------------------
// LCD_Delay
//-----------------------------------------------------------------------------
//
void LCD_Delay(int time) {
	int n, m;
	for (n = 0; n < time; n++) {
		//for (m=0;m<20;m++);
	}
//HAL_Delay(time);
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
	/* USER CODE BEGIN 2 */
	//HAL_Delay(1000);
	SSD1306_Init();  // initialise

	//HAL_Delay(1000);

	/// lets print some string

	SSD1306_Clear();
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts("HELLO", &Font_11x18, 1);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts("  WORLD :)", &Font_11x18, 1);
	SSD1306_UpdateScreen(); //display
	SSD1306_ON();

	// SSD1306_Clear();

	HAL_Delay(2000);

	SSD1306_Clear();
	SSD1306_GotoXY(10, 0);
	SSD1306_Puts("Font 7x10", &Font_7x10, 1);
	SSD1306_GotoXY(10, 14);
	SSD1306_Puts("11x18", &Font_11x18, 1);
	SSD1306_GotoXY(10, 35);
	SSD1306_Puts("16x26", &Font_16x26, 1);
	SSD1306_UpdateScreen(); //display
	SSD1306_ON();

	HAL_Delay(5000);

	//SSD1306_ScrollRight(0,7);  // scroll entire screen
	//HAL_Delay(2000);  // 2 sec

	//SSD1306_ScrollLeft(0,7);  // scroll entire screen
	//HAL_Delay(2000);  // 2 sec

	//SSD1306_Stopscroll();

	SSD1306_Clear();

	SSD1306_DrawBitmap(0, 0, logo, 128, 64, 1);
	SSD1306_UpdateScreen();

	HAL_Delay(2000);

	//SSD1306_ScrollRight(0x00, 0x0f);    // scroll entire screen right

	//HAL_Delay (2000);

	//SSD1306_ScrollLeft(0x00, 0x0f);  // scroll entire screen left

	//HAL_Delay (2000);

	//SSD1306_Scrolldiagright(0x00, 0x0f);  // scroll entire screen diagonal right

	//HAL_Delay (2000);

	//SSD1306_Scrolldiagleft(0x00, 0x0f);  // scroll entire screen diagonal left

	//HAL_Delay (2000);

	//SSD1306_Stopscroll();   // stop scrolling. If not done, screen will keep on scrolling

	SSD1306_InvertDisplay(1);   // invert the display

	HAL_Delay(2000);

	SSD1306_InvertDisplay(0);  // normalize the display
	//

	HAL_Delay(2000);

	TestLines(1);
	HAL_Delay(1000);
	TestRectangles(1);
	HAL_Delay(1000);
	TestFilledRectangles(1);
	HAL_Delay(1000);
	TestCircles(6, 1);
	HAL_Delay(1000);
	TestFilledCircles(6, 1);
	TestTriangles(1);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);

	/*Configure GPIO pin : PA0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
