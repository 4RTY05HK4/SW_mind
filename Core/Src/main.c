/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "M8_Disp.h"
#include "queue.h"
#include "semphr.h"
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
 struct Conf Conf1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

QueueHandle_t keypadQueue = NULL;
QueueHandle_t dispQueue = NULL;

SemaphoreHandle_t readKeypadSemaphore;
SemaphoreHandle_t keyPressedSemaphore;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void KeyLOGIC( void * pvParameters )
{
	uint8_t keycode = 0;
	uint16_t counter = 10000;
	uint8_t flag = 0;
	char *code = '0';

	while(1)
	{
		//if( readKeypadSemaphore != NULL )
		//{


					keycode = decode();

					if (keycode > 0 && keycode < 11 && flag != 0)
					{
						--keycode;
						if( xSemaphoreTake( readKeypadSemaphore, ( TickType_t ) 10 ) == pdTRUE )
						{
							if(xQueueSendToBack(keypadQueue, (void*)&keycode, (TickType_t)10) == pdPASS)
							{
								//sprintf(&code, "%01d", keycode);
								//HAL_UART_Transmit(&huart2, &code, 2, 10);
								//xSemaphoreGive( keyPressedSemaphore );
							}
						}
					flag = 0;
					}
					else if(!keycode && !flag)
					{
						if(!counter)
						{
							counter = 10000;
							flag = 1;
						}
					counter--;
					}

		//}
	}

}


void DispLOGIC( void * pvParameters )
{
	uint8_t digits[10] = {0};
	uint8_t buffer = 0;
	char *code =  '0';
	uint8_t counter = 0;
	uint8_t signs = 0;

	while(1)
	{
	   if(dispQueue != NULL)
	   {
		  //digits[counter] = 0;
	      if(xQueueReceive(dispQueue, &(buffer), (TickType_t)10) == pdPASS )
	      {
	    	  	switch(buffer)
	    	  	{

	    	  	case 108:
	    	  		Disp_Clear(Conf1);
	    	  		Disp_Write_Word(Conf1, "LOSS", 4);
	    	  		break;
	    	  	case 119:
	    	  		Disp_Clear(Conf1);
	    	  		Disp_Write_Word(Conf1, "WIN", 3);
	    	  		break;
	    	  	default:
		    	  	digits[counter] = buffer;
					sprintf(&code, "%1d%1d%1d%1d", digits[counter], digits[counter-1], digits[counter-2], digits[counter-3]);
					//HAL_UART_Transmit(&huart2, &code, 4, 10);
					Disp_Write_Word(Conf1, &code, counter+1);
					//Disp_Write_Word_Shift(Conf1, "12345", 5);
					//code = ' ';
					counter++;
					if(counter > signs)
					{
						signs++;
						counter = 0;
					}

					break;

	    	  	}
	      }
	   }
	}
}

int * generateRandArray()
{
	char *code = '0';
	static int tab[10];
	//HAL_UART_Transmit(&huart2, "generate", 8, 10);
	for(uint8_t i=0; i<=10 ;i++){
		HAL_ADC_Start(&hadc1);
		vTaskDelay(1);
		tab[i] = HAL_ADC_GetValue(&hadc1)%10;
		//sprintf(&code, "%d", tab[i]);
		//HAL_UART_Transmit(&huart2, &code, 1, 10);
	}
	//Disp_Write_Word_Shift(Conf1, &code, 10);
	return tab;
}


void mainLOGIC( void * pvParameters )
{

	uint8_t buffer = 0;
	uint8_t check = 0;
	char *code = '0';
	char codeToUser[10] = {0};
	uint8_t step = 0;
	uint8_t stage = 0;
	uint8_t progres = 4; // 0 - lose/ 1 - game in progress/ 2 - win/ 4 - diff select
	uint8_t diff = 5;
	uint8_t randomlyGeneratedArray[10];
	int *pointerTorandomlyGeneratedArray;
	pointerTorandomlyGeneratedArray = generateRandArray();
	//HAL_UART_Transmit(&huart2, "copied", 6, 10);
	for (uint8_t i = 0; i <= 10; i++ ) {
		randomlyGeneratedArray[i] = *(pointerTorandomlyGeneratedArray+i);
		//sprintf(&code, "%1d", randomlyGeneratedArray[i]);
		//HAL_UART_Transmit(&huart2, &code, 1, 10);
	}
	//sprintf(&codeToUser, "%1d", randomlyGeneratedArray[0]);
	//Disp_Write_Word_Shift(Conf1, &codeToUser, 1);

	while(1)
	{

		if(keypadQueue != NULL) //&& keyPressedSemaphore != NULL)
		{

		  if(xQueueReceive(keypadQueue, &(buffer), (TickType_t)10) == pdPASS )
		  {
				//HAL_UART_Transmit(&huart2, "Received", 8, 10);
				//sprintf(&code, "%1d", buffer);
				//HAL_UART_Transmit(&huart2, &code, 1, 10);
				//code = ' ';
				if(progres == 4) {
					diff = buffer + 1;
					progres = 1;
					xSemaphoreGive( readKeypadSemaphore );
					sprintf(&codeToUser, "%1d", randomlyGeneratedArray[0]);
					Disp_Write_Word_Shift(Conf1, &codeToUser, 1);
				}
				else if(progres == 1 && (buffer == randomlyGeneratedArray[stage]) && (step <= diff-1))
				{
					if(xQueueSendToBack(dispQueue, (void*)&buffer, (TickType_t)10) == pdPASS)
					{
						//sprintf(&code, "%01d", keycode);
						//HAL_UART_Transmit(&huart2, "Sent", 4, 10);
						//sprintf(&code, "%1d", buffer);
						//HAL_UART_Transmit(&huart2, &code, 1, 10);
						stage++;
						xSemaphoreGive( readKeypadSemaphore );
						if(stage == step+1)
						{
							step++;
							if(step <= diff-1){
								vTaskDelay(500);
								sprintf(&code, "%d", randomlyGeneratedArray[step]);
								strcat(&codeToUser, &code);
								Disp_Write_Word_Shift(Conf1, &codeToUser, step+1);
								stage = 0;
							}
						}
					}
				}
				else if(step >= diff) progres = 2;
				else progres = 0;
		  }
	   }
		if(step >= diff) progres = 2;
		//else if(step < diff) progres = 0;
		if(progres == 2)
		{
			if(xQueueSendToBack(dispQueue, (void*)"w", (TickType_t)10) == pdPASS)
			{
				//HAL_UART_Transmit(&huart2, "Sent", 4, 10);
				//HAL_UART_Transmit(&huart2, "WIN", 3, 10);
				xSemaphoreGive( readKeypadSemaphore );
			}
		}
		else if(progres == 0)
		{
			if(xQueueSendToBack(dispQueue, (void*)"l", (TickType_t)10) == pdPASS)
			{
				//HAL_UART_Transmit(&huart2, "Sent", 4, 10);
				//HAL_UART_Transmit(&huart2, "LOSS", 4, 10);
				xSemaphoreGive( readKeypadSemaphore );
			}
		}
	}
}
//vQueueDelete(keypadQueue);




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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  Conf1.GPIO_Pin=GPIO_PIN_6;
  Conf1.GPIOx=GPIOB;
  Conf1.hspi=hspi1;
  Disp_Init(Conf1, 0x01);

  xTaskCreate(
		  	  	  	  mainLOGIC,       /* Function that implements the task. */
                      "MAIN",          /* Text name for the task. */
                      1000,      /* Stack size in words, not bytes. */
                      NULL,    /* Parameter passed into the task. */
                      1,/* Priority at which the task is created. */
                      NULL );      /* Used to pass out the created task's handle. */

  xTaskCreate(
		  	  	  	  KeyLOGIC,       /* Function that implements the task. */
                      "KEY",          /* Text name for the task. */
                      1000,      /* Stack size in words, not bytes. */
                      NULL,    /* Parameter passed into the task. */
                      1,/* Priority at which the task is created. */
                      NULL );      /* Used to pass out the created task's handle. */

  xTaskCreate(
		  	  	  	  DispLOGIC,       /* Function that implements the task. */
                      "DISP",          /* Text name for the task. */
                      1000,      /* Stack size in words, not bytes. */
                      NULL,    /* Parameter passed into the task. */
                      1,/* Priority at which the task is created. */
                      NULL );      /* Used to pass out the created task's handle. */




  keypadQueue = xQueueCreate( 10, sizeof(uint8_t));
  if(keypadQueue == 0) HAL_UART_Transmit(&huart2, "Err_queue", 9, 10);
  dispQueue = xQueueCreate( 10, sizeof(uint8_t));
  if(dispQueue == 0) HAL_UART_Transmit(&huart2, "Err_queue", 9, 10);

  readKeypadSemaphore = xSemaphoreCreateBinary();
  keyPressedSemaphore = xSemaphoreCreateBinary();

  xSemaphoreGive( readKeypadSemaphore );

  Disp_Clear(Conf1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//scanRows();
		//uint8_t keycode = decode();
		//HAL_UART_Transmit(&huart2, &keycode, 2, 10);
		//char *code = '0';
		//sprintf(&code, "%01d", keycode);
		//HAL_UART_Transmit(&huart2, &code, 2, 10);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
