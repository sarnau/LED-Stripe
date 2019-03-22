/**
  ******************************************************************************
  * @file    Libraries/EasySTM32Lib/color.c
  * @author  Matthias Hagmann
  * @version V1.0
  * @date    11-February-2016
  * @brief   Set of functions to manage and convert colors
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "color.h"
#include <math.h>
#include <stdint.h>

/**
  * @brief  Returns red quantity
  * @param  color:
  *   24-bit RGB color.
  * @retval red 
  */
uint8_t getRed(uint32_t color) {
  return (color & 0xFF0000) >> 16;
}

/**
  * @brief  Returns green quantity
  * @param  color:
  *   24-bit RGB color.
  * @retval green 
  */
uint8_t getGreen(uint32_t color) {
  return (color & 0xFF00) >> 8;
}

/**
  * @brief  Returns blue quantity
  * @param  color:
  *   24-bit RGB color.
  * @retval blue 
  */
uint8_t getBlue(uint32_t color) {
  return (color & 0xFF);
}

/**
  * @brief  Sets red quantity
  * @param  red:
  *   8-bit color.
  * @retval 24-bit color 
  */
uint32_t setRed(uint8_t red) {
  return red << 16;
}

/**
  * @brief  Sets green quantity
  * @param  green:
  *   8-bit color.
  * @retval 24-bit color 
  */
uint32_t setGreen(uint8_t green) {
  return (green << 8);
}

/**
  * @brief  Sets blue quantity
  * @param  blue:
  *   8-bit color.
  * @retval 24-bit color 
  */
uint32_t setBlue(uint8_t blue) {
  return blue;
}

/**
  * @brief  Sets color
  * @param  red:	8-bit color
  *  		green:	8-bit color
  *  		blue:	8-bit color
  * @retval 24-bit color 
  */
uint32_t setColor(uint8_t red, uint8_t green, uint8_t blue) {
  return (setRed(red) | setGreen(green) | setBlue(blue));
}

//-------------------------------------------------------------------------------
//HSV nach RGB konvertieren
//-------------------------------------------------------------------------------
uint32_t hsv_to_rgb(uint16_t hue, uint8_t saturation, uint8_t value) {
  float h = hue, s = saturation, v = value;

  int i;
  int r_, g_, b_;
  float r, g, b;
  float f, p, q, t;

  s /= 100;
  v /= 100;

  if (s == 0) {
    r = v;
    g = v;
    b = v;
  } else {
    h /= 60; // sector 0 to 5
    i = floor(h);
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;

    case 1:
      r = q;
      g = v;
      b = p;
      break;

    case 2:
      r = p;
      g = v;
      b = t;
      break;

    case 3:
      r = p;
      g = q;
      b = v;
      break;

    case 4:
      r = t;
      g = p;
      b = v;
      break;

    default: // case 5
      r = v;
      g = p;
      b = q;
      break;
    }
  }
  r_ = r * 255;
  g_ = g * 255;
  b_ = b * 255;

  //printf("R: %02X G: %02X B: %02X\n", r_, g_, b_);

  // Auf 8 Bit RGB skalieren
  return setRed(r_) | setGreen(g_) | setBlue(b_);
}

// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
void rgb_to_hsv(float *h, float *s, float *v, uint32_t rgb) {
  float min, max, delta;
  float r, g, b;

  r = (float)getRed(rgb) / 255;
  g = (float)getGreen(rgb) / 255;
  b = (float)getBlue(rgb) / 255;

  // minimum
  if (r > g)
    min = g;
  else
    min = r;
  if (min > b)
    min = b;

  // maximum
  if (r > g)
    max = r;
  else
    max = g;
  if (max < b)
    max = b;

  *v = max; // v
  delta = max - min;
  if (max != 0)
    *s = delta / max; // s
  else {
    // r = g = b = 0		// s = 0, v is undefined
    *s = 0;
    *h = 0; // instead of -1
    return;
  }
  if (r == max)
    *h = (g - b) / delta; // between yellow & magenta
  else if (g == max)
    *h = 2 + (b - r) / delta; // between cyan & yellow
  else
    *h = 4 + (r - g) / delta; // between magenta & cyan
  *h *= 60;                   // degrees
  if (*h < 0)
    *h += 360;

  *s *= 100;
  *v *= 100;
}

/******************* (C) COPYRIGHT 2016 Matthias Hagmann *****END OF FILE****/