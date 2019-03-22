/**
  ******************************************************************************
  * @file    Libraries/EasySTM32Lib/color.h
  * @author  Matthias Hagmann
  * @version V1.0
  * @date    11-February-2016
  * @brief   Set of functions to manage and convert colors
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_COLOR_H
#define STM32_COLOR_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>

/* Definitions ---------------------------------------------------------------*/
#define RED 0xFF0000
#define ORANGE 0xFF7F00
#define YELLOW 0xFFFF00
#define GREEN 0x00FF00
#define LIGHTBLUE 0x00FFFF
#define BLUE 0x0000FF
#define VIOLET 0x8F00FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000

/* Prototypes ----------------------------------------------------------------*/
uint32_t hsv_to_rgb(uint16_t hue, uint8_t saturation, uint8_t value);
void rgb_to_hsv(float *h, float *s, float *v, uint32_t rgb);

uint8_t getRed(uint32_t color);
uint8_t getGreen(uint32_t color);
uint8_t getBlue(uint32_t color);
uint32_t setColor(uint8_t red, uint8_t green, uint8_t blue);
uint32_t setRed(uint8_t red);
uint32_t setGreen(uint8_t green);
uint32_t setBlue(uint8_t blue);

#endif /* STM32_COLOR_H */

/******************* (C) COPYRIGHT 2014 Matthias Hagmann *****END OF FILE****/