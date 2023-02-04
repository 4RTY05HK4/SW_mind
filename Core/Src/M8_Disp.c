/*
 * M8_Disp.c
 *
 */
#include "M8_Disp.h"

const uint8_t digits[][8]={
		{0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},      // Space  - indeks 0
	   {0x04 , 0x0E , 0x0E , 0x04 , 0x04 , 0x00 , 0x04 , 0x00},      //!        indeks 1
	   {0x1B , 0x1B , 0x12 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},      //?
	   {0x00 , 0x0A , 0x1F , 0x0A , 0x0A , 0x1F , 0x0A , 0x00},      //#
	   {0x08 , 0x0E , 0x10 , 0x0C , 0x02 , 0x1C , 0x04 , 0x00},      //$
	   {0x19 , 0x19 , 0x02 , 0x04 , 0x08 , 0x13 , 0x13 , 0x00},      //%
	   {0x08 , 0x14 , 0x14 , 0x08 , 0x15 , 0x12 , 0x0D , 0x00},      //&
	   {0x0C , 0x0C , 0x08 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},      //
	   {0x04 , 0x08 , 0x08 , 0x08 , 0x08 , 0x08 , 0x04 , 0x00},      //
	   {0x08 , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x08 , 0x00},      //
	   {0x00 , 0x0A , 0x0E , 0x1F , 0x0E , 0x0A , 0x00 , 0x00},      //*
	   {0x00 , 0x04 , 0x04 , 0x1F , 0x04 , 0x04 , 0x00 , 0x00},      //+
	   {0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x0C , 0x0C , 0x08},      //,
	   {0x00 , 0x00 , 0x00 , 0x1F , 0x00 , 0x00 , 0x00 , 0x00},      //-
	   {0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x0C , 0x0C , 0x00},      //.
	   {0x00 , 0x01 , 0x02 , 0x04 , 0x08 , 0x10 , 0x00 , 0x00},      ///
	   {0x0E , 0x11 , 0x13 , 0x15 , 0x19 , 0x11 , 0x0E , 0x00},      //0
	   {0x04 , 0x0C , 0x04 , 0x04 , 0x04 , 0x04 , 0x0E , 0x00},      //1
	   {0x0E , 0x11 , 0x01 , 0x06 , 0x08 , 0x10 , 0x1F , 0x00},      //2
	   {0x0E , 0x11 , 0x01 , 0x0E , 0x01 , 0x11 , 0x0E , 0x00},      //3
	   {0x02 , 0x06 , 0x0A , 0x12 , 0x1F , 0x02 , 0x02 , 0x00},      //4
	   {0x1F , 0x10 , 0x10 , 0x1E , 0x01 , 0x11 , 0x0E , 0x00},      //5
	   {0x06 , 0x08 , 0x10 , 0x1E , 0x11 , 0x11 , 0x0E , 0x00},      //6
	   {0x1F , 0x01 , 0x02 , 0x04 , 0x08 , 0x08 , 0x08 , 0x00},      //7
	   {0x0E , 0x11 , 0x11 , 0x0E , 0x11 , 0x11 , 0x0E , 0x00},      //8
	   {0x0E , 0x11 , 0x11 , 0x0F , 0x01 , 0x02 , 0x0C , 0x00},      //9
	   {0x00 , 0x00 , 0x0C , 0x0C , 0x00 , 0x0C , 0x0C , 0x00},      //:
	   {0x00 , 0x00 , 0x0C , 0x0C , 0x00 , 0x0C , 0x0C , 0x08},      //;
	   {0x02 , 0x04 , 0x08 , 0x10 , 0x08 , 0x04 , 0x02 , 0x00},      //<
	   {0x00 , 0x00 , 0x1F , 0x00 , 0x00 , 0x1F , 0x00 , 0x00},      //=
	   {0x08 , 0x04 , 0x02 , 0x01 , 0x02 , 0x04 , 0x08 , 0x00},      //>
	   {0x0E , 0x11 , 0x01 , 0x06 , 0x04 , 0x00 , 0x04 , 0x00},      //?
	   {0x0E , 0x11 , 0x17 , 0x15 , 0x17 , 0x10 , 0x0E , 0x00},      //@
	   {0x0E , 0x11 , 0x11 , 0x1F , 0x11 , 0x11 , 0x11 , 0x00},      //A
	   {0x1E , 0x11 , 0x11 , 0x1E , 0x11 , 0x11 , 0x1E , 0x00},      //B
	   {0x0E , 0x11 , 0x10 , 0x10 , 0x10 , 0x11 , 0x0E , 0x00},      //C
	   {0x1E , 0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x1E , 0x00},      //D
	   {0x1F , 0x10 , 0x10 , 0x1E , 0x10 , 0x10 , 0x1F , 0x00},      //E
	   {0x1F , 0x10 , 0x10 , 0x1E , 0x10 , 0x10 , 0x10 , 0x00},      //F
	   {0x0E , 0x11 , 0x10 , 0x17 , 0x11 , 0x11 , 0x0F , 0x00},      //G
	   {0x11 , 0x11 , 0x11 , 0x1F , 0x11 , 0x11 , 0x11 , 0x00},      //H
	   {0x0E , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x0E , 0x00},      //I
	   {0x01 , 0x01 , 0x01 , 0x01 , 0x11 , 0x11 , 0x0E , 0x00},      //J
	   {0x11 , 0x12 , 0x14 , 0x18 , 0x14 , 0x12 , 0x11 , 0x00},      //K
	   {0x10 , 0x10 , 0x10 , 0x10 , 0x10 , 0x10 , 0x1F , 0x00},      //L
	   {0x11 , 0x1B , 0x15 , 0x11 , 0x11 , 0x11 , 0x11 , 0x00},      //M
	   {0x11 , 0x19 , 0x15 , 0x13 , 0x11 , 0x11 , 0x11 , 0x00},      //N
	   {0x0E , 0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x0E , 0x00},      //O
	   {0x1E , 0x11 , 0x11 , 0x1E , 0x10 , 0x10 , 0x10 , 0x00},      //P
	   {0x0E , 0x11 , 0x11 , 0x11 , 0x15 , 0x12 , 0x0D , 0x00},      //Q
	   {0x1E , 0x11 , 0x11 , 0x1E , 0x12 , 0x11 , 0x11 , 0x00},      //R
	   {0x0E , 0x11 , 0x10 , 0x0E , 0x01 , 0x11 , 0x0E , 0x00},      //S
	   {0x1F , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x00},      //T
	   {0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x0E , 0x00},      //U
	   {0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x0A , 0x04 , 0x00},      //V
	   {0x11 , 0x11 , 0x15 , 0x15 , 0x15 , 0x15 , 0x0A , 0x00},      //W
	   {0x11 , 0x11 , 0x0A , 0x04 , 0x0A , 0x11 , 0x11 , 0x00},      //X
	   {0x11 , 0x11 , 0x11 , 0x0A , 0x04 , 0x04 , 0x04 , 0x00},      //Y
	   {0x1E , 0x02 , 0x04 , 0x08 , 0x10 , 0x10 , 0x1E , 0x00},      //Z
	   {0x0E , 0x08 , 0x08 , 0x08 , 0x08 , 0x08 , 0x0E , 0x00},      //[
	   {0x00 , 0x10 , 0x08 , 0x04 , 0x02 , 0x01 , 0x00 , 0x00},      // "\"
	   {0x0E , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x0E , 0x00},      //]
	   {0x04 , 0x0A , 0x11 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},      //^
	   {0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x1F},      //_
	   {0x0C , 0x0C , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},      //`
	   {0x00 , 0x00 , 0x0E , 0x01 , 0x0F , 0x11 , 0x0F , 0x00},      //a
	   {0x10 , 0x10 , 0x1E , 0x11 , 0x11 , 0x11 , 0x1E , 0x00},      //b
	   {0x00 , 0x00 , 0x0E , 0x11 , 0x10 , 0x11 , 0x0E , 0x00},      //c
	   {0x01 , 0x01 , 0x0F , 0x11 , 0x11 , 0x11 , 0x0F , 0x00},      //d
	   {0x00 , 0x00 , 0x0E , 0x11 , 0x1F , 0x10 , 0x0E , 0x00},      //e
	   {0x06 , 0x08 , 0x08 , 0x1E , 0x08 , 0x08 , 0x08 , 0x00},      //f
	   {0x00 , 0x00 , 0x0F , 0x11 , 0x11 , 0x0F , 0x01 , 0x0E},      //g
	   {0x10 , 0x10 , 0x1C , 0x12 , 0x12 , 0x12 , 0x12 , 0x00},      //h
	   {0x04 , 0x00 , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x00},      //i
	   {0x02 , 0x00 , 0x06 , 0x02 , 0x02 , 0x02 , 0x12 , 0x0C},      //j
	   {0x10 , 0x10 , 0x12 , 0x14 , 0x18 , 0x14 , 0x12 , 0x00},      //k
	   {0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x06 , 0x00},      //l
	   {0x00 , 0x00 , 0x1A , 0x15 , 0x15 , 0x11 , 0x11 , 0x00},      //m
	   {0x00 , 0x00 , 0x1C , 0x12 , 0x12 , 0x12 , 0x12 , 0x00},      //n
	   {0x00 , 0x00 , 0x0E , 0x11 , 0x11 , 0x11 , 0x0E , 0x00},      //o
	   {0x00 , 0x00 , 0x1E , 0x11 , 0x11 , 0x11 , 0x1E , 0x10},      //p
	   {0x00 , 0x00 , 0x0F , 0x11 , 0x11 , 0x11 , 0x0F , 0x01},      //q
	   {0x00 , 0x00 , 0x16 , 0x09 , 0x08 , 0x08 , 0x1C , 0x00},      //r
	   {0x00 , 0x00 , 0x0E , 0x10 , 0x0E , 0x01 , 0x0E , 0x00},      //s
	   {0x00 , 0x08 , 0x1E , 0x08 , 0x08 , 0x0A , 0x04 , 0x00},      //t
	   {0x00 , 0x00 , 0x12 , 0x12 , 0x12 , 0x16 , 0x0A , 0x00},      //u
	   {0x00 , 0x00 , 0x11 , 0x11 , 0x11 , 0x0A , 0x04 , 0x00},      //v
	   {0x00 , 0x00 , 0x11 , 0x11 , 0x15 , 0x1F , 0x0A , 0x00},      //w
	   {0x00 , 0x00 , 0x12 , 0x12 , 0x0C , 0x12 , 0x12 , 0x00},      //x
	   {0x00 , 0x00 , 0x12 , 0x12 , 0x12 , 0x0E , 0x04 , 0x18},      //y
	   {0x00 , 0x00 , 0x1E , 0x02 , 0x0C , 0x10 , 0x1E , 0x00},      //z
	   {0x06 , 0x08 , 0x08 , 0x18 , 0x08 , 0x08 , 0x06 , 0x00},      //{
	   {0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x04 , 0x00},      //|
	   {0x0C , 0x02 , 0x02 , 0x03 , 0x02 , 0x02 , 0x0C , 0x00}      //}};
};

const uint8_t sep[8]={0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00};

void SPI_Send(struct Conf C,uint8_t address, uint8_t D){
	HAL_SPI_Transmit(&C.hspi, &address, 1, 100);
	HAL_SPI_Transmit(&C.hspi, &D, 1, 100); //  wysłanie danych
}

void Disp_Init(struct Conf C,uint8_t brightness){ 	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	SPI_Send(C,0x0c, 0x00);     //  wył =0，wł = 1
	SPI_Send(C,0x0c, 0x00);
	SPI_Send(C,0x0c, 0x00);
	SPI_Send(C,0x0c, 0x00);
	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);


	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	SPI_Send(C,0x0f, 0x00);		//  test wyświetlacza
	SPI_Send(C,0x0f, 0x00);
	SPI_Send(C,0x0f, 0x00);
	SPI_Send(C,0x0f, 0x00);
	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	SPI_Send(C,0x09, 0x00);       //  dekodowanie
	SPI_Send(C,0x09, 0x00);
	SPI_Send(C,0x09, 0x00);
	SPI_Send(C,0x09, 0x00);
	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	SPI_Send(C,0x0a, brightness);       //  jasność
	SPI_Send(C,0x0a, brightness);
	SPI_Send(C,0x0a, brightness);
	SPI_Send(C,0x0a, brightness);
	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	SPI_Send(C,0x0b, 0x07);       //  scan limit = 8 LED
	SPI_Send(C,0x0b, 0x07);
	SPI_Send(C,0x0b, 0x07);
	SPI_Send(C,0x0b, 0x07);
	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	SPI_Send(C,0x0c, 0x01);       //  wył =0，wł = 1
	SPI_Send(C,0x0c, 0x01);
	SPI_Send(C,0x0c, 0x01);
	SPI_Send(C,0x0c, 0x01);
	HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);


}

void Disp_Write(struct Conf C,uint8_t address, uint8_t *D) {/**Zapal wszystkie koplumny wyświetlacza*/


HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
	for (uint8_t j=0;j<4;j++){
		SPI_Send(C,address,D[j]);
	}
HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);  // CS w stanie wysokim

}

void Disp_Clear(struct Conf C)	{ // Czyszczenie wyświetlacza

	for(int i=1;i<=8;i++){
			HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_RESET);
			SPI_Send(C,i,0x00);
			SPI_Send(C,i,0x00);
			SPI_Send(C,i,0x00);
			SPI_Send(C,i,0x00);
			HAL_GPIO_WritePin(C.GPIOx, C.GPIO_Pin, GPIO_PIN_SET);
			}

}

void Disp_Write_Word(struct Conf C,char *D, uint8_t size) { // Wyświetlanie ciągu stojącego
	uint8_t data[4];
	for (uint8_t i=0;i<8;i++){
			for (uint8_t j=0;j<4;j++){
				if(j>size-1) data[j] = (digits[0][i]);
				else data[j] = (digits[D[j]-32][i]);
			}
			Disp_Write(C,i+1,data);
		}
}

void Disp_Write_Word_Shift(struct Conf C,char *D,uint8_t size) { // Wyświetlanie ciągu przesuwnego
	size +=4;
	uint8_t dataout[4];
	uint16_t datam[size][8];
	uint8_t help=0;

	for (uint8_t i=0;i<8;i++){
				for (uint8_t j=0;j<size;j++){
					if(j<4) datam[j][i] = (digits[0][i]);
					else datam[j][i] = (digits[D[j-4]-32][i]);
				}
			}


	for(uint8_t x=0;x<size;x++){
		for (uint8_t i=0;i<8;i++){

				for (uint8_t j=0;j<8;j++){
					for (uint8_t k=0;k<size;k++){
						datam[k][j]=datam[k][j]<<1;
					}
							if (datam[1][j]>255){
								datam[1][j]=datam[1][j]-256;
								datam[0][j]++;
							}

							if (datam[2][j]>255){
								datam[2][j]=datam[2][j]-256;
								datam[1][j]++;
							}

							if (datam[3][j]>255){
								datam[3][j]=datam[3][j]-256;
								datam[2][j]++;
							}
							if (datam[4][j]>255){
								datam[4][j]=datam[4][j]-256;
								datam[3][j]++;
							}

							dataout[0]=datam[0][j]& 0xff;
							dataout[1]=datam[1][j]& 0xff;
							dataout[2]=datam[2][j]& 0xff;
							dataout[3]=datam[3][j]& 0xff;
							Disp_Write(C,j+1,dataout);
							}
		vTaskDelay(100);
						}
if(5+help<size){
	for (uint8_t i=0;i<8;i++){
		datam[4][i] = (digits[D[1+help]-32][i]);
	}
		help++;
}

	}


}

void Disp_Write_Time(struct Conf C,char *D) { // Wyświetlanie czasu
		uint8_t data[4];
		for (uint8_t i=0;i<8;i++){
				for (uint8_t j=0;j<4;j++){
					data[j] = (digits[D[j]-32][i]);
					if(j==2){
						data[j]=data[j]+sep[i];
					}
				}

				Disp_Write(C,i+1,data);
			}
	}





