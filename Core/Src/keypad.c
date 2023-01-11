/*
 * keypad.c
 *
 *  Created on: Jan 11, 2023
 *      Author: ThinkPad-X1
 */

#include "keypad.h"

volatile uint8_t wyj = 0b00000001;
volatile uint32_t counter = 0;
volatile uint8_t wej = 0;

uint8_t scanRows(){

	if(wej == 0)
	{
	        if(counter == 100)
	        {
	            counter = 0;
	            wyj = wyj * 2;
	            if(wyj > 8)
	            {
	                wyj = 1;
	            }
	        }
	        counter = counter + 1;
	}

	switch(wyj) {
	  case 1:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	    break;
	  case 2:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	    break;
	  case 4:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	    break;
	  case 8:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	    break;

	  default:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	return wyj;
}

uint8_t readCols(){

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)) {
		wej = 1;
		return 0b0001;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) {
		wej = 2;
		return 0b0010;
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) {
		wej = 4;
		return 0b0100;
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)) {
		wej = 8;
		return 0b1000;
	}
	wej = 0;
	return 0;
}

uint8_t decode(){
	uint8_t cols = readCols();
	uint8_t rows = wyj;
	uint8_t rejMS = 0;
	uint8_t rejLS = 0;

	if(cols != 0)
	{
	        switch(rows){
				case 1 :
					rejMS = 0b0000;
					break;
				case 2 :
					rejMS = 0b0100;
					break;
				case 4 :
					rejMS = 0b1000;
					break;
				case 8 :
					rejMS = 0b1100;
					break;
				default: rejMS = 0b00;
	        }

	        switch(cols){
				case 1 :
					rejLS = 0;
					break;
				case 2 :
					rejLS = 1;
					break;
				case 4 :
					rejLS = 0b10;
					break;
				case 8 :
					rejLS = 0b11;
					break;
				default: rejLS = 0b00;
	        }
		return rejMS + rejLS + 1;
	}

	return 0;

}

