/*
 * keypad.h
 *
 *  Created on: Jan 11, 2023
 *      Author: ThinkPad-X1
 */

#ifndef SRC_KEYPAD_H_
#define SRC_KEYPAD_H_

#include "gpio.h"
#include "stm32f4xx_hal.h"

uint8_t scanRows();
uint8_t readCols();
uint8_t decode();




#endif /* SRC_KEYPAD_H_ */
