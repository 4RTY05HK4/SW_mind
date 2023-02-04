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

/** \mainpage Mind game
 *
 * \section intro Założenia
 *
 * Projekt zakłada stworzenie gry w oparciu o mikrokontroler STM32 wspierający system FreeRTOS, oraz peryfiera w postaci
 * klawiatury matrycowej i wyświetlacz matrycowy LED.
 *
 * \section rules Zasady
 * Po wybraniu poziomu trudności na wyświetlaczu pojawi się pierwszy element z sekwencji przykładowych znaków
 * (wylosowanych z określonego zbioru dziesięciu cyfr od 0 do 9), zadaniem gracza jest zapamiętanie tej sekwencji
 * i po zakończeniu jej wyświetlania odtworzenie kolejności znaków za pośrednictwem klawiatury (każdy znak posiada
 * przyporządkowany jeden przycisk). W każdym etapie gry zwiększana jest liczba znaków do zapamiętania (modyfikacja
 * może także polegać na zwiększaniu zbioru możliwych znaków do zapamiętania). Gra kończy się w momencie, kiedy
 * gracz popełni pomyłkę podczas wprowadzania sekwencji znaków lub gdy wprowadzi wszystkie znaki które zostały wylosowane.
 * Grę można w dowolnym momencie przerwać poprzez naciśnięcie przycisku Start.
 *
 * \section perypherials Peryferia
 *
 * \subsection rand Generator liczb losowych
 * Generowanie liczb w sposób losowy odbywa się za pośrednictwem przetwornika ADC w funkcji generateRandArray().
 * Za jego pomocą zczytywane są wartości z tzw. "wiszącego kabla", które następnie konwertowane są na liczby od 0 do 9.
 *
 * \subsection led Wyświetlacz matrycowy LED
 * Obsługę wyświetlacza matrycowego realizuje zadanie DispLOGIC() przy pomocy biblioteki M8_Disp.h stworzonej na potrzebę innych zajęć.
 *
 * \subsection keypad Klawiatura matrycowa
 * Obsługę klawiatury matrycowej realizuje zadanie KeyLOGIC() przy pomocy biblioteki keypad.h stworzonej na potrzebę projektu.
 *
 * \section tasks Zadania
 * Projekt został podzielony na 3 zadania: KeyLOGIC(), DispLOGIC() oraz mainLOGIC(), odpowiadające kolejno obsłudze klawiatury matrycowej,
 * obsłudze wyświetlacza matrycowego LED oraz głównej logice programu.
 *
 *
 *
 *
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
#include "keypad.h"
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
 QueueHandle_t keypadQueue = NULL;
 QueueHandle_t dispQueue = NULL;

 SemaphoreHandle_t readKeypadSemaphore;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/**
 * \brief Funkcja zadania systemu FreeRTOS obsługująca odczyt z klawiatury matrycowej.
 * Funkcja decode() zwraca kod (liczbę od 1 do 16) wciśniętego przycisku. Zadanie sprawdza kod przycisku,
 * ponieważ ze względu na charakter projektu używane jest tylko 10 pierwszych klawiszy, a następnie kod
 * przycisku konwertowany jest na wartość, która zapisywana jest do kolejki keypadQueue, pod warunkiem
 * że ustawiony jest semafor readKeypadSemaphore.
 */
void KeyLOGIC( void * pvParameters ){

	uint8_t keycode = 0;
	uint16_t counter = 10000;
	uint8_t flag = 0;
	char *code = '0';

	while(1){
		keycode = decode();

		if (keycode > 0 && keycode < 11 && flag != 0){
			--keycode;
			if( xSemaphoreTake( readKeypadSemaphore, ( TickType_t ) 10 ) == pdTRUE ){
				if(xQueueSendToBack(keypadQueue, (void*)&keycode, (TickType_t)10) == pdPASS){}
			}
		flag = 0;
		}
		else if(!keycode && !flag){
			if(!counter){
				counter = 10000;
				flag = 1;
			}
		counter--;
		}
	}
}

/**
 * \brief Funkcja zadania systemu FreeRTOS obsługująca wyświetlacz matrycowy LED.
 * \details Funkcja odczytuje dane otrzymane z kolejki dispQueue i wyświetla je przy użyciu funkcji Disp_Write_Word()
 * z biblioteki M8_Disp. W przypadku otrzymania wartości odpowiadającej literom "l" lub "w",
 * funkcja wyświetla odpowiednie słowa komunikujące przegraną lub wygraną.
 */
void DispLOGIC( void * pvParameters ){

	uint8_t digits[10] = {0};
	uint8_t buffer = 0;
	char *code =  '0';
	uint8_t counter = 0;
	uint8_t signs = 0;

	while(1){
		if(dispQueue != NULL){
			if(xQueueReceive(dispQueue, &(buffer), (TickType_t)10) == pdPASS ){
				switch(buffer){

					case 108:
						Disp_Clear(Conf1);
						Disp_Write_Word(Conf1, "LOSS", 4);
						vTaskDelay(500);
						Disp_Clear(Conf1);
						vTaskDelay(250);
						break;
					case 119:
						Disp_Clear(Conf1);
						Disp_Write_Word(Conf1, "WIN", 3);
						vTaskDelay(250);
						Disp_Clear(Conf1);
						vTaskDelay(250);
						break;
					default:
						digits[counter] = buffer;
						sprintf(&code, "%1d%1d%1d%1d", digits[counter], digits[counter-1], digits[counter-2], digits[counter-3]);
						Disp_Write_Word(Conf1, &code, counter+1);
						counter++;
						if(counter > signs){
							signs++;
							counter = 0;
						}
						break;
				}
			}
		}
	}
}

/**
 * \brief Funkcja generująca tablicę 10 losowych liczb.
 * \details W celu wygenerowania liczb losowychużyto przetwornika ADC. Odczytuje on szumy z wiszącego wyprowadzenia
 * mikrokontrolera, które następnie zamieniane są na liczby.
 * z zakresu od 0 do 9.
 */
int * generateRandArray(){
	char *code = '0';
	static int tab[10];

	for(uint8_t i=0; i<=10 ;i++){
		HAL_ADC_Start(&hadc1);
		vTaskDelay(1);
		tab[i] = HAL_ADC_GetValue(&hadc1)%10;
	}

	return tab;
}

/**
 * \brief Funkcja zadania systemu FreeRTOS obsługująca całą logikę programu.
 * \details Przed rozpoczęciem rozgrywki, generowana jest tablica
 * zawierająca 10 losowych cyfr (0-9) przy użyciu funkcji generateRandArray(). W głównej pętli zadanie oczekuje na otrzymanie
 * kodu przycisku za pośrednictwem kolejki keypadQueue. Przy pierwszym uruchomieniu zadanie będzie oczekiwać na wciśnięcie
 * klawisza od 1 do 10 oznaczającego wybór poziomu trudności. Gdy gracz wybierze poziom trudności wówczas zadanie
 * rozpocznie grę, wysyłając pierwszą losową wygenerowaną liczbę, po czym będzie oczekiwać na wprowadzenie przez użytkownika
 * odpowiedniej wartości z klawiatury.
 * Porównanie wartości wprowadzonej z klawiatury oraz wartości znajdującej się w wygenerowanej wcześniej tablicy odbywa się
 * poprzez porównanie zmiennej buffer z odpowiednią komórka tablicy randomlyGeneratedArray w instrukcji warunkowej.
 * Po odebraniu danej z kolejki i jej przetworzeniu następuje oddanie semafora readKeypadSemaphore, aby gracz mógł wprowdzać kolejne cyfry.
 * Gra może skończyć się rezultatem przegranym gdy wprowadzona wartość nie będzie spełniała warunku, wtedy do kolejki dispQueue
 * zostaje wysłany znak "l", widziany jako kod ASCII. W przypadku wygranej, czyli gdy zmienna step zrówna się z zmienną diff, do kolejki
 *  dispQueue, zostaje wysłany znak "w". O stanie gry informuje zmienna progres.
 * Ciąg znaków, jaki ma zostać wysłany do sterownika jest poddawany konkatenacji przy pomocy strcat z kolejnymi wartościami
 * z wygenerowanej tablicy po zakończeniu każdego etapu gry.
 *
 * uint8_t buffer - Zmienna buforowa przechowywująca ostatni znak pobrany z kolejki.
 *
 * char *code - Zmienna wskaźnikowa przechowywująca ostatni znak pobrany z kolejki w formacie znakowym.
 *
 * char codeToUser - Ciąg znaków wyświetlany kierowany na sterownik wyświetlacza.
 *
 * uint8_t step - Aktualny poziom, na którym znajduje się gracz (długość ciągu do zapamiętania).
 *
 * uint8_t stage - Iteracja po elementach wygenerowanej tablicy podczas porównania.
 *
 * uint8_t progress - Stan gry: 0 - lose/ 1 - game in progress/ 2 - win/ 4 - diff select.
 *
 * uint8_t diff - Poziom trudności domyślnie ustawiony na 5.
 */
void mainLOGIC( void * pvParameters ){
	uint8_t buffer = 0;
	char *code = '0';
	char codeToUser[10] = {0};
	uint8_t step = 0;
	uint8_t stage = 0;
	uint8_t progres = 4;
	uint8_t diff = 5;

	uint8_t randomlyGeneratedArray[10];
	int *pointerTorandomlyGeneratedArray;
	pointerTorandomlyGeneratedArray = generateRandArray();

	for (uint8_t i = 0; i <= 10; i++ ) {
		randomlyGeneratedArray[i] = *(pointerTorandomlyGeneratedArray+i);
	}

	while(1){
		if(keypadQueue != NULL){
			if(xQueueReceive(keypadQueue, &(buffer), (TickType_t)10) == pdPASS ){
				if(progres == 4) {
					diff = buffer + 1;
					progres = 1;
					xSemaphoreGive( readKeypadSemaphore );
					sprintf(&codeToUser, "%1d", randomlyGeneratedArray[0]);
					Disp_Write_Word_Shift(Conf1, &codeToUser, 1);
				}
				else if(progres == 1 && (buffer == randomlyGeneratedArray[stage]) && (step <= diff-1)){
					if(xQueueSendToBack(dispQueue, (void*)&buffer, (TickType_t)10) == pdPASS){
						stage++;
						xSemaphoreGive( readKeypadSemaphore );
						if(stage == step+1){
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
		if(progres == 2){
			if(xQueueSendToBack(dispQueue, (void*)"w", (TickType_t)10) == pdPASS){
				xSemaphoreGive( readKeypadSemaphore );
			}
		}
		else if(progres == 0){
			if(xQueueSendToBack(dispQueue, (void*)"l", (TickType_t)10) == pdPASS){
				xSemaphoreGive( readKeypadSemaphore );
			}
		}
	}
}

/* USER CODE END 0 */

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
		  	  	  	  mainLOGIC,      	// Funkcja realizująca zadania
                      "MAIN",          	// Nazwa zadania
                      1000,      		// Rozmiar stosu przeznaczony na zadanie
                      NULL,    			// Parametr przekazany do zadania
					  1,				// Priorytet zadania
                      NULL );     		// Używany do przekazania uchwytu zadania

  xTaskCreate(
		  	  	  	  KeyLOGIC,       	// Funkcja realizująca zadania
                      "KEY",          	// Nazwa zadania
                      1000,      		// Rozmiar stosu przeznaczony na zadanie
                      NULL,    			// Parametr przekazany do zadania
                      1,				// Priorytet zadania
                      NULL );      		// Używany do przekazania uchwytu zadania

  xTaskCreate(
		  	  	  	  DispLOGIC,       	// Funkcja realizująca zadania
                      "DISP",           // Nazwa zadania
                      1000,      		// Rozmiar stosu przeznaczony na zadanie
                      NULL,    			// Parametr przekazany do zadania
                      1,				// Priorytet zadania
					  NULL );      		// Używany do przekazania uchwytu zadania




  keypadQueue = xQueueCreate( 10, sizeof(uint8_t)); // Utworzenie kolejki 10 elementowej dla przesyłu kodów klawiszy.
  if(keypadQueue == 0) HAL_UART_Transmit(&huart2, "Err_queue", 9, 10);
  dispQueue = xQueueCreate( 10, sizeof(uint8_t)); // Utworzenie kolejki 10 elementowej dla przesyłu znaków do wyświetlenia
  if(dispQueue == 0) HAL_UART_Transmit(&huart2, "Err_queue", 9, 10);

  readKeypadSemaphore = xSemaphoreCreateBinary(); // Utworzenie semafora zezwalającego na zapis kodu klawisza do kolejki
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
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief Konfiguracja zegara systemowego.
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
  * @brief  Funkcja wykonywana podczas błędu systemu.
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
