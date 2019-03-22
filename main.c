/**
  ******************************************************************************
  * @file    Project/LED-Stripe/main.c 
  * @author  Matthias Hagmann
  * @version V1.0
  * @date    11-February-2016
  * @brief   WS2812B LED stripe demo project with StdPeriphLib
	*					 Microcontroller runninng at 24MHz
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "color.h"
#include "rgb_lib.h"
#include "stm32f10x.h"

int main(void) {
  uint16_t h = 0, s = 100, v = 25;

  init_stripe();

  // Infinite loop
  while (1) {
    uint32_t color = hsv_to_rgb(h, s, v);
    set_stripe(stripe, color);
    refresh_stripe(stripe, NR_OF_LEDS);

    // constant color change
    h++;
    h %= 360;

    // delay
    for (int i = 0; i < 150000; i++) {
    }
  }
}

/******************* (C) COPYRIGHT 2016 Matthias Hagmann *****END OF FILE****/