/**
 * M8_Disp.h
 *
 *  Created on: Dec 2, 2022
 *      Author: Kamil
 *  \brief    Biblioteka Obslugi wyswietlacza
 *  Bilblioteka obsługująca 4 połacone szeregowo wyświetlacze ledowe 8*8
 *
 */

#ifndef SRC_M8_DISP_H_
#define SRC_M8_DISP_H_
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32f446xx.h"
#include "stm32f4xx_hal_gpio.h"







/**
 * \brief Struktura konfiguracyjna
 * \details Struktura zawiera odpowiednie elementy potrzebe w celu poprawnego wyboru systemowego spi oraz pinu CS
 * \param[in]   hspi		Systemowe SPI
 * \param[in]	GPIOx		Port CS
 * \param[in]	GPIO_Pin	Pin CS
 */
struct Conf{
	SPI_HandleTypeDef hspi;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
};


/**
 * \brief Funkcja wysyłająca dane z wykorzystaniem SPI
 * \details Funkcja wysyłająca pod wskazany adres daną, która ma zostać wysłana. Nie uwzględa przy tym odpowiedniego ustawienia sygnalu CS
 * \param[in]   C		Struktura konfiguracyjna
 * \param[in]	address		adres pod który ma zostać wysłana dana
 * \param[in]	Data		Dana, która ma zostać wysłana
 */
void SPI_Send(struct Conf C,uint8_t address, uint8_t Data);

/**
 * \brief Funkcja inicjalizująca wyświetlacz i ustawiająca jego jasność
 * \details Funkcja inicjalizująca wyświetlacz oraz ustawiająca wskazana jasnosć
 * \param[in]    C		Struktura konfiguracyjna
 * \param[in]	brightness	ustaiwenie jasności ekranu w zakresie 0-15
 */
void Disp_Init(struct Conf,uint8_t brightness);

/**
 * \brief Funkcja czyszcząca wyświetlacz
 * \details Funkcja gasząca wszytkie zapalone diody
 * \param[in]   Conf C		Struktura konfiguracyjna
 */
void Disp_Clear(struct Conf C);

/**
 * \brief Funkcja wyświetlająca rząd na 4 wyświetlaczach
 * \details Funkcja gasząca wszytkie zapalone diody
 * \param[in]   Conf C		Struktura konfiguracyjna
 * \param[in]	address		adres pod który ma zostać wysłana dana
 * \param[in]	Data		Dana, która ma zostać wysłana()
 *
 */
void Disp_Write(struct Conf,uint8_t address, uint8_t *Data);

/**
 * \brief Funkcja wyświetlająca 4 znaki(słowo)
 * \details Funkcja wyświetlająca na ekranie słowo w postaci 4 znaków na podstaiwe tabeli znaków
 * \param[in]   Conf C		Struktura konfiguracyjna
 * \param[in] 	D			Wskaźnik na słowo
 */
void Disp_Write_Word(struct Conf C,char *D,uint8_t size);

/**
 * \brief Funkcja wyświetlająca czas
 * \details Funkcja wyświetlająca na ekranie czas w postaci 4 znaków odzeloych dwukropkiem pomiędzy minutami i godzinami
 * \param[in]   Conf C		Struktura konfiguracyjna
 * \param[in] 	D			Wskaźnik na słowo(czas)
 */
void Disp_Write_Time(struct Conf C,char *D);

/**
 * \brief Funkcja wyświetlająca napis przesuwając go od prawej do lewej
 * \details Funkcja wyświetlająca na ekranie czas w postaci 4 znaków odzeloych dwukropkiem pomiędzy minutami i godzinami
 * \param[in]   Conf C		Struktura konfiguracyjna
 * \param[in] 	D			Wskaźnik na słowo
 * \param[in]	size		Długosć słowa
 */
void Disp_Write_Word_Shift(struct Conf C,char *D,uint8_t size);

#endif /* SRC_M8_DISP_H_ */
