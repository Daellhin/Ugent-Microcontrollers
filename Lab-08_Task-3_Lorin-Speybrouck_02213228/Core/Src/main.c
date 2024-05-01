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
#include <math.h>
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_accelero.h"
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

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
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
void LCD_Delay(int time); //just a predeclaration

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

void I2Cstart(void) {
	PCF_SDA(1);
	PCF_SCL(1); //I2C start condition
	PCF_SDA(0); //high to low data while clk high
	PCF_SCL(0);
}

void I2Cstop(void) {
	//PCF_SDA(0);
	PCF_SCL(1); //I2C stop condition
	PCF_SDA(1); //low to high data while clk high
}

//MSB first for PCF chip!!!
uint8_t I2Cwritebyte(uint8_t databyte) {
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
// LCD_Init   Robust version written by P. Van Torre DEC 2014
//-----------------------------------------------------------------------------
//
// Position of parallel LCD lines in byte output by PCF chip
// from MSB to LSB
// D7 D6 D5 D4 BL E R/W RS
// BL= backlight, active high
//
// Check datasheet of HD44780 LCD controller for more info

//-----------------------------------------------------------------------------
// LCD_Delay
//-----------------------------------------------------------------------------
//
void LCD_Delay(int time) {
	int n, m;
	for (n = 0; n < time; n++) {
		for (m = 0; m < 20; m++)
			;
	}
	//HAL_Delay(time);
}

//-----------------------------------------------------------------------------
// LCD_Clock
//-----------------------------------------------------------------------------
//
void LCD_Clock(char lcddata) {
	I2Cwritebyte((lcddata & 0xf0) | ((lcddata & 0x02) >> 1) | 0x08); //4 bit data + RS line + BL
	LCD_Delay(10);
	I2Cwritebyte((lcddata & 0xf0) | ((lcddata & 0x02) >> 1) | 0x0C); //4 bit data + RS line + BL + EN
	LCD_Delay(10);
	I2Cwritebyte((lcddata & 0xf0) | ((lcddata & 0x02) >> 1) | 0x08); //4 bit data + RS line + BL
	LCD_Delay(10);

}

void LCD_Init(void) {
	//wait a while after power on
	LCD_Delay(2550);

	//go to 8-bit mode no matter what (8 -> 8 / 4 -> 8)
	LCD_Clock(0x30);
	LCD_Delay(2550);
	LCD_Clock(0x30);
	LCD_Delay(250);
	LCD_Clock(0x30);
	LCD_Delay(250);

	//now go to 4-bit mode
	LCD_Clock(0x20);
	LCD_Delay(250);

	//interface definitely in 4-bit mode now, independent of history

	//4-bit 2-line
	LCD_Clock(0x20);
	LCD_Clock(0x80);
	LCD_Delay(250);

	//no shift ; cursor right
	LCD_Clock(0x10);
	LCD_Clock(0x40);
	LCD_Delay(250);

	//display on ; cursor off ; no blinking
	LCD_Clock(0x00);
	LCD_Clock(0xC0);
	LCD_Delay(250);

	//cursor moves right ; no display shift
	LCD_Clock(0x00);
	LCD_Clock(0x60);
	LCD_Delay(250);
}

//-----------------------------------------------------------------------------
// LCD_Busy PVT2014
//-----------------------------------------------------------------------------
//
void LCD_Busy(void) {
	LCD_Delay(1);
	//original busy function can probably not be used with the LCD shield
	//due to R/W line which is always on WRITE ???

}

//-----------------------------------------------------------------------------
// LCD_Send
//-----------------------------------------------------------------------------
//
void LCD_Send(char teken) {
	LCD_Clock((teken & 0xf0) | 2);  // |2 used to set RS=1
	LCD_Clock(((teken & 0x0f) << 4) | 2);
	LCD_Busy(); //wait until LCD ready
}

//-----------------------------------------------------------------------------
// LCD_String pvantorre 2020
//-----------------------------------------------------------------------------
//
void LCD_String(char *text) {
	while (*text != 0)
		LCD_Send(*text++);
}

//-----------------------------------------------------------------------------
// LCD_Clear
//-----------------------------------------------------------------------------
//
void LCD_Clear(void) {
	LCD_Clock(0x00);
	LCD_Clock(0x10);
	LCD_Delay(255);
}

//-----------------------------------------------------------------------------
// LCD_Home
//-----------------------------------------------------------------------------
//
void LCD_Home(void) {
	LCD_Clock(0x00);
	LCD_Clock(0x20);
}

//-----------------------------------------------------------------------------
// LCD_Pos
//-----------------------------------------------------------------------------
//
void LCD_Pos(unsigned char pos) {
	LCD_Clock(0x80 | (pos & 0xf0));
	LCD_Clock(pos << 4);
	LCD_Busy();
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */
	float getValueFromState(int state) {
		switch (state) {
		case 0:
			return BSP_TSENSOR_ReadTemp();
		case 1:
			return BSP_HSENSOR_ReadHumidity();
		case 2:
			return BSP_PSENSOR_ReadPressure();
		default:
			int16_t XYZ[3];
			BSP_ACCELERO_AccGetXYZ(XYZ);
			return (float) XYZ[state - 2];

		}
	}
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
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	BSP_TSENSOR_Init(); // HTS221
	BSP_HSENSOR_Init(); // HTS221
	BSP_PSENSOR_Init(); // LPS22HB
	BSP_ACCELERO_Init(); // LSM6DSL
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	int state = 0;
	char *states[] = { "Temperature", "Humidity", "Pressure", "X", "Y", "Z" };
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		float value = getValueFromState(state);

		char text[200] = "";
		sprintf(text, "State %d| %s: %f.2 \n\r", state, states[state], value);
		HAL_UART_Transmit(&huart1, (uint8_t*) text, sizeof(text), 1000);

		I2Cstart();
		I2Cwritebyte(0x27 << 1);
		LCD_Init();
		LCD_Clear();

		LCD_Pos(0);
		LCD_String(states[state]);
		LCD_Pos(40);
		char LCDText[50] = "";
		sprintf(LCDText, "%.4f", value);
		LCD_String(LCDText);
		I2Cstop();

		HAL_Delay(1000);
		state = (state + 1) % 6;
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
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA0 PA1 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PD10 PD11 PD15 */
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
