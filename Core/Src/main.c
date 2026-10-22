/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include "ds18b20.h"

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void SystemClock_Config(void);

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
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  ssd1306_SetCursor(10, 20);
  ssd1306_WriteString("Phase 2 OK", Font_11x18, White);
  ssd1306_UpdateScreen();
  DWT_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  uint32_t last_reset_tick = 0;
	  const uint32_t reset_interval = 1000; // 1 second interval

	  while (1)
	  {
	    /* USER CODE END WHILE */

	    /* USER CODE BEGIN 3 */
	      // 1. Check the physical button continuously (instantly responsive)
	      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
	      {
	          // Button pressed
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // LED ON
	          ssd1306_Fill(Black);
	          ssd1306_SetCursor(10, 20);
	          ssd1306_WriteString("BTN PRESSED!", Font_7x10, White);
	          ssd1306_UpdateScreen();
	      }
	      else
	      {
	          // Button released (Idle)
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // LED OFF
	      }

	      // 2. Non-blocking timer check for live DS18B20 temperature
	      if ((HAL_GetTick() - last_reset_tick) >= 2000)
	      {
	          last_reset_tick = HAL_GetTick(); // Reset timer

	          // Kick off conversion and read directly
	          DS18B20_StartConversion();
	          delay_us(750000); // Wait 750ms for conversion

	          float temp_c = DS18B20_ReadTemperature();

	          // Split float into whole and fractional parts
	          int temp_whole = (int)temp_c;
	          int temp_fract = (int)((temp_c - temp_whole) * 100);
	          if (temp_fract < 0) temp_fract = -temp_fract;

	          // Format and display live temperature
	          char temp_str[32];
	          sprintf(temp_str, "Temp: %d.%02d C", temp_whole, temp_fract);

	          ssd1306_Fill(Black);
	          ssd1306_SetCursor(10, 10);
	          ssd1306_WriteString("Thermal Node", Font_7x10, White);
	          ssd1306_SetCursor(10, 30);
	          ssd1306_WriteString(temp_str, Font_7x10, White);
	          ssd1306_UpdateScreen();

	      // Small delay to prevent overwhelming the CPU and I2C bus
	          HAL_Delay(10);
	          /* USER CODE END 3 */
	      }           // <--- Closes the while (1) loop

	  }               // <--- Closes the main() function
}
	  /**
	    * @brief System Clock Configuration
	    */
	  void SystemClock_Config(void)
	  {
	      // ...

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
}
  /* USER CODE END Error_Handler_Debug */

#ifdef USE_FULL_ASSERT
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
