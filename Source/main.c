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