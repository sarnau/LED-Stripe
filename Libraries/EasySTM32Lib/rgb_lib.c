/**
  ******************************************************************************
  * @file    Libraries/EasySTM32Lib/rgb_lib.c
  * @author  Matthias Hagmann
  * @version V1.0
  * @date    11-February-2016
  * @brief   Set of low level functions to control WS2812B Leds
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rgb_lib.h"
#include "color.h"
#include "stm32f10x.h"

uint32_t stripe[NR_OF_LEDS];
uint8_t spiDataBuffer[BUFFER_SIZE];

// private
volatile uint8_t spiBusyFlag = 0; // SPI busy

/**
  * @brief  Configures SPI1 and DMA1 channel 3 for 
  * 				800kHz Ouput Signal (WS2812B RGB Leds).
  * @param  None
  * @retval None
  */
void init_stripe(void) {
  // Enable GPIO, SPI1, DMA1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  // Configure SPI Tx and SCLK as alternate function push-pull
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // SPI configuration
  /* SPIx configured as follow:
	    - BaudRate Prescaler = 4
	    - Data Size = 8 Bits
	    - Transmit enabled
	*/
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CRCPolynomial = 0;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_Init(SPI1, &SPI_InitStructure);

  // Enable SPIx/I2Sx DMA
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

  // Enable SPI
  SPI_Cmd(SPI1, ENABLE);

  // DMA configuration
  /* DMAy Channelx configured as follow:
	    - Buffer Size = 0
		- Direction = Peripheral -> Destination
		- Memory Base Address = spiDataBuffer
		- Data Size = Byte
		- Peripheral Increment disabled
	*/
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spiDataBuffer;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  // Enable DMAy Channelx Transmit Complete interrupt
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

  // NVIC configuration
  /* NVIC configured as follow:
	 	- DMA = 1
		- channel = 3
		- priority = 0
	*/
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // 15_03_05 workaround: 1st bit error
  // Disable DMAy Channelx
  DMA_Cmd(DMA1_Channel3, DISABLE);
  DMA_SetCurrDataCounter(DMA1_Channel3, 1);

  spiDataBuffer[0] = WS2812B_RESET;

  // Enable DMA Channelx
  DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
  * @brief  Set color of a LED
  * @param  stripe:		array holding color of all leds  
* @param  color:			values [8x0 8xR 8xG 8xB]
  * @param  number:	number of LED to set, starts with 0
  * @retval None
  */
void set_led(uint32_t *stripe, uint8_t pixel, uint32_t color) {
  if (pixel < NR_OF_LEDS) {
    stripe[pixel] = color;
  }
}

/**
  * @brief  Set color of all LEDs
  * @param  stripe:		array holding color of all leds
  * @param  color:		values [8x0 8xR 8xG 8xB]
  * @retval None
  */
void set_stripe(uint32_t *stripe, uint32_t color) {
  for (int i = 0; i < NR_OF_LEDS; i++) {
    stripe[i] = color;
  }
}

/**
  * @brief  Fills transmit buffer and sends data via DMA1 channel 3 on SPI1.
  * @param  uint32_t:	color values [R G B]
  * @param  number:	number of LEDs to refresh
  * @retval None
  */
void refresh_stripe(uint32_t *stripe, uint16_t leds) {
  while (spiBusyFlag) // spi ready
  {
  }
  spiBusyFlag = 1;

  // Disable DMA
  DMA_Cmd(DMA1_Channel3, DISABLE);
  DMA_SetCurrDataCounter(DMA1_Channel3, leds * 24 + BYTE_RESET); // 3 Byte pro LED + 40 Byte Reset

  // Data
  for (int led_nr = 0; led_nr < leds; led_nr++) {
    uint32_t led_color = *(stripe + led_nr); // 00 RR GG BB
    for (int bit = 0; bit <= 7; bit++) {
      // G
      if (((getGreen(led_color) >> (8 - bit)) & 0x01) == 1) // High
      {
        spiDataBuffer[led_nr * 24 + bit] = WS2812B_HIGH;
      } else // Low
      {
        spiDataBuffer[led_nr * 24 + bit] = WS2812B_LOW;
      }
      // R
      if (((getRed(led_color) >> (8 - bit)) & 0x01) == 1) // High
      {
        spiDataBuffer[led_nr * 24 + bit + 8] = WS2812B_HIGH;
      } else // Low
      {
        spiDataBuffer[led_nr * 24 + bit + 8] = WS2812B_LOW;
      }
      // B
      if (((getBlue(led_color) >> (8 - bit)) & 0x01) == 1) // High
      {
        spiDataBuffer[led_nr * 24 + bit + 16] = WS2812B_HIGH;
      } else // Low
      {
        spiDataBuffer[led_nr * 24 + bit + 16] = WS2812B_LOW;
      }
    }
  }

  // 50us Reset
  for (int bit = 0; bit < BYTE_RESET; bit++) {
    spiDataBuffer[leds * 24 + bit] = WS2812B_RESET;
  }

  // DMA enable
  DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
  * @brief  Handles SPI1 transmit finished Interrupt.
  * @param  None
  * @retval None
  */
void handle_SPI_int(void) {
  spiBusyFlag = 0;
}