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
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "MCP23S17.h"
#include "SGTL5000.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAI_TX_BUFFER_LENGTH (48*2)
#define SAI_RX_BUFFER_LENGTH (48*2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static int16_t sai_tx_buffer[SAI_TX_BUFFER_LENGTH];
static int16_t sai_rx_buffer[SAI_RX_BUFFER_LENGTH];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int	__io_putchar(int t)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&t, 1, 100);
	return (t);
}

void	testLed()
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(500); // attendre 500ms
	return ;
}
void	testUart()
{
	printf("Bonjour, je suis un test \r\n");
	return ;
}

void	testGPIOExpander()
{
	int i = 0;
	int state = 1;
	while (i < 8)
	{
		MCP23S17_SetPin('A', i, 0);
		MCP23S17_SetPin('B', i, 0);

		state *= 2;
		i++;
		HAL_Delay(1000);
	}
	MCP23S17_SetMultiplePins('A', 0xff, 0xff);
	MCP23S17_SetMultiplePins('B', 0xff, 0xff);
	return ;
}

static int rx_cplt_flag = 0;
static int rx_cplt_counter = 0;

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
	if (SAI2_Block_B == hsai->Instance)
	{
		rx_cplt_flag = 1;

		for (int i = SAI_RX_BUFFER_LENGTH / 2 ; i < SAI_RX_BUFFER_LENGTH ; i++)
		{
			sai_tx_buffer[i] = sai_rx_buffer[i];
		}
	}
	rx_cplt_counter++;
}
static int rx_half_cplt_flag = 0;
static int rx_half_counter = 0;
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
	if (SAI2_Block_B == hsai->Instance)
	{
		rx_half_cplt_flag = 1;

		for (int i = 0 ; i < SAI_RX_BUFFER_LENGTH / 2 ; i++)
		{
			sai_tx_buffer[i] = sai_rx_buffer[i];
		}
		rx_half_counter++;
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

	/* Configure the peripherals common clocks */
	PeriphCommonClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_SPI3_Init();
	MX_I2C2_Init();
	MX_SAI2_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(nRST_GPIO_Port, nRST_Pin, 1);
	MCP23S17_Init(&hspi3);
	sgtl5000_init(&hi2c2);
	HAL_SAI_Receive_DMA(&hsai_BlockB2, (uint8_t*) sai_rx_buffer, SAI_RX_BUFFER_LENGTH / 2);
	HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t*) sai_tx_buffer, SAI_TX_BUFFER_LENGTH / 2);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	uint8_t led_max;
	int ave_counter = 0;
	int ave = 0;
	int ave_max = 100;
	while (1)
	{
		uint8_t data[2];
		HAL_I2C_Mem_Read(&hi2c2, 0x14, 0x0000, 2, data, 2, 0x400);
		printf("I2C read = 0x%X, 0x%X\n\r", data[0], data[1]);
		if (rx_cplt_flag)
		{
			rx_cplt_flag = 0;

			ave += sai_rx_buffer[0];
			ave_counter++;

			if (ave_counter == ave_max)
			{

				ave /= ave_max;
				if (ave < 0) ave = -ave;

				led_max = (ave/5 * 8) / 100;
				if (led_max > 8) led_max = 8;
				for (uint8_t i = 0; i < 8; i++)
				{
					MCP23S17_SetPin('A', i, i < led_max);
					MCP23S17_SetPin('B', i, i < led_max);
				}

				ave = 0;
				ave_counter = 0;
			}
		}
		//testLed(); // Fonction test LED
		//testUart();
		//testGPIOExpander();
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

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
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
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
	PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 13;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
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
