#include "main.h"
#include <string.h>

UART_HandleTypeDef huart1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

// EXTI Line0 External Interrupt ISR Handler CallBackFun
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)           (Declared in stm32f4xx_hal_gpio.h)


void ledBlinksShortPress(uint16_t numberOfBlinks);
void ledBlinksLongPress(uint16_t numberOfBlinks);

uint32_t lastDebounceTime = 0;	//record the time for last interrupt occurrence
uint32_t longPressTime = 1000;  //1000ms or 1s (time for long press of push button)
uint32_t shortPressTime = 200;	//200ms (time for short press of push button)
uint8_t buttonPressed = 0;
uint8_t shortPressed = 0;

uint16_t dotFlag = 0;			//Morse code dot
uint16_t dashFlag = 0;			//Morse code dash

uint8_t charAcquire[24] = {0};	//Array of flags


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  while (1)
  {
	  //record one short-press [E]
	  if(!charAcquire[0])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}

			if(dotFlag)
			{
				charAcquire[0] = 1;
				dotFlag = 0;

				char buf[8] = "E";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record two long-press [M]
	  else if(charAcquire[0] && !charAcquire[1])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 2)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag == 2)
			{
				charAcquire[1] = 1;
				dashFlag = 0;

				char buf[8] = "M";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record short-long-short press [R]
	  else if(charAcquire[1] && !charAcquire[2])
	  {

			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}
			if(dotFlag)
			{
				charAcquire[2] = 1;
				dotFlag = 0;
			}
	  }

	  else if(charAcquire[2] && !charAcquire[3])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 1)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag)
			{
				charAcquire[3] = 1;
				dashFlag = 0;
			}
	  }

	  else if(charAcquire[3] && !charAcquire[4])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}
			if(dotFlag)
			{
				charAcquire[4] = 1;
				dotFlag = 0;

				char buf[8] = "R";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record short press [E]
	  else if(charAcquire[4] && !charAcquire[5])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}
			if(dotFlag)
			{
				charAcquire[5] = 1;
				dotFlag = 0;

				char buf[8] = "E ";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record three long press [O]
	  else if(charAcquire[5] && !charAcquire[6])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 3)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag == 3)
			{
				charAcquire[6] = 1;
				dashFlag = 0;

				char buf[8] = "O";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record two long press,two short press [Z]
	  else if(charAcquire[6] && !charAcquire[7])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 2)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag == 2)
			{
				charAcquire[7] = 1;
				dashFlag = 0;
			}
	  }

	  else if(charAcquire[7] && !charAcquire[8])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag++;
				if(dotFlag == 2)
					ledBlinksShortPress(dotFlag);

				buttonPressed = 0;
			}
			if(dotFlag ==2)
			{
				charAcquire[8] = 1;
				dotFlag = 0;

				char buf[8] = "Z";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record a long press [T]
	  else if(charAcquire[8] && !charAcquire[9])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 1)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag)
			{
				charAcquire[9] = 1;
				dashFlag = 0;

				char buf[8] = "T";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record two short press, one long press [U]
	  else if(charAcquire[9] && !charAcquire[10])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag++;
				if(dotFlag == 2)
					ledBlinksShortPress(dotFlag);

				buttonPressed = 0;
			}
			if(dotFlag ==2)
			{
				charAcquire[10] = 1;
				dotFlag = 0;

				char buf[8] = "U";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  else if(charAcquire[10] && !charAcquire[11])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 1)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag)
			{
				charAcquire[11] = 1;
				dashFlag = 0;

				char buf[8] = "R";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  // record one short press, one long press, one short press [R]
	  else if(charAcquire[11] && !charAcquire[12])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}
			if(dotFlag)
			{
				charAcquire[12] = 1;
				dotFlag = 0;
			}
	  }

	  else if(charAcquire[12] && !charAcquire[13])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 1)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag)
			{
				charAcquire[13] = 1;
				dashFlag = 0;
			}
	  }

	  else if(charAcquire[13] && !charAcquire[14])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}
			if(dotFlag)
			{
				charAcquire[14] = 1;
				dotFlag = 0;

				char buf[8] = "R";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

	  //record one long press, one short press, one long press [K]
	  else if(charAcquire[14] && !charAcquire[15])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 1)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag)
			{
				charAcquire[15] = 1;
				dashFlag = 0;
			}
	  }

	  else if(charAcquire[15] && !charAcquire[16])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > shortPressTime)
			{
				dotFlag = 1;
				ledBlinksShortPress(dotFlag);
			}
			if(dotFlag)
			{
				charAcquire[16] = 1;
				dotFlag = 0;
			}
	  }

	  else if(charAcquire[16] && !charAcquire[17])
	  {
			if(buttonPressed && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) && (HAL_GetTick() - lastDebounceTime) > longPressTime)
			{
				dashFlag++;
				if(dashFlag == 1)
					ledBlinksLongPress(dashFlag);

				buttonPressed = 0;
			}

			if(dashFlag)
			{
				charAcquire[17] = 1;
				dashFlag = 0;

				char buf[8] = "K";
				HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
			}
	  }

  }

}

void ledBlinksShortPress(uint16_t numberOfBlinks)
{
	for(int i = 0; i < numberOfBlinks; i++)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(500);
	}
}

void ledBlinksLongPress(uint16_t numberOfBlinks)
{
	for(int i = 0; i < numberOfBlinks; i++)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		HAL_Delay(2000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		HAL_Delay(500);
	}
}

// EXTI Line0 External Interrupt ISR Handler CallBackFun
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	lastDebounceTime = HAL_GetTick();  //Get time when interrupt occurs
	buttonPressed = 1;
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
