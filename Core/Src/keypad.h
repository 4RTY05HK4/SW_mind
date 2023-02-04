/*
 * keypad.h
 *
 */

#ifndef SRC_KEYPAD_H_
#define SRC_KEYPAD_H_

#include "gpio.h"
#include "stm32f4xx_hal.h"

/**
 * \brief Skanowanie klawiatury
 * \details Funkcja ustawiająca krążącą jedynkę na wierszach wyświetlacza.
 */
uint8_t scanRows();

/**
 * \brief Odczyt z Klawiatury
 * \details Funkcja odczytująca stan kolumn.
 */
uint8_t readCols();

/**
 * \brief Dekodowanie stanów
 * \details Funkcja dekodująca dane wierszy i kolumn klwaiatury na ciąg wyjściowy.
 */
uint8_t decode();




#endif /* SRC_KEYPAD_H_ */
