/**
  ******************************************************************************
  * @file    Libraries/EasySTM32Lib/rgb_lib.h
  * @author  Matthias Hagmann
  * @version V1.0
  * @date    11-February-2016
  * @brief   Set of low level functions to control WS2812B Leds
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_RGB_H
#define STM32_RGB_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Definitions ---------------------------------------------------------------*/
#define NR_OF_LEDS 150
#define BYTE_RESET 40
#define BUFFER_SIZE NR_OF_LEDS * 8 * 3 + BYTE_RESET
//#define WS2812B_HIGH 0x03 // not inverted
//#define WS2812B_LOW 0x3F  // not inverted
// use with 74HCT04
#define WS2812B_HIGH 0xFC // inverted
#define WS2812B_LOW 0xC0  // Inverted
#define WS2812B_RESET 0xFF

extern uint32_t stripe[NR_OF_LEDS];

/* Prototypes ----------------------------------------------------------------*/
void init_stripe(void);
void set_led(uint32_t *stripe, uint8_t pixel, uint32_t color);
void set_stripe(uint32_t *stripe, uint32_t color);
void refresh_stripe(uint32_t *stripe, uint16_t leds);
void handle_SPI_int(void);

// !!! IMPORTANT !!!  - Uncomment and add this to stm32f10x_it.c
/*
#include "rgb_lib.h"
void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_FLAG_TC3) == SET)
	{
		handle_SPI_int();
		DMA_ClearFlag(DMA1_FLAG_TC3);
	}
}
*/

#endif /* STM32_RGB_H */

/******************* (C) COPYRIGHT 2014 Matthias Hagmann *****END OF FILE****/