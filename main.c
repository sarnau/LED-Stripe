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

static void delay(int d) {
  for (int i = 0; i < d * 1000; i++) {
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return setColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return setColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return setColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (int i = 0; i < NR_OF_LEDS; i++) {
    set_led(stripe, i, c);
    refresh_stripe(stripe, NR_OF_LEDS);
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < NR_OF_LEDS; i++) {
      set_led(stripe, i, Wheel((i + j) & 255));
    }
    refresh_stripe(stripe, NR_OF_LEDS);
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  for (int j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (int i = 0; i < NR_OF_LEDS; i++) {
      set_led(stripe, i, Wheel(((i * 256 / NR_OF_LEDS) + j) & 255));
    }
    refresh_stripe(stripe, NR_OF_LEDS);
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NR_OF_LEDS; i = i + 3) {
        set_led(stripe, i + q, c); //turn every third pixel on
      }
      refresh_stripe(stripe, NR_OF_LEDS);

      delay(wait);

      for (int i = 0; i < NR_OF_LEDS; i = i + 3) {
        set_led(stripe, i + q, 0); //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) { // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NR_OF_LEDS; i = i + 3) {
        set_led(stripe, i + q, Wheel((i + j) % 255)); //turn every third pixel on
      }
      refresh_stripe(stripe, NR_OF_LEDS);

      delay(wait);

      for (int i = 0; i < NR_OF_LEDS; i = i + 3) {
        set_led(stripe, i + q, 0); //turn every third pixel off
      }
    }
  }
}

int main(void) {
  init_stripe();

  while (1) {
    const int speed[5] = {50, 25, 10, 4, 0};
    for (int loop = 0; loop < 5; ++loop) {
      colorWipe(setColor(255, 255, 255), speed[loop]);
      delay(1000);
      colorWipe(setColor(0, 0, 0), speed[loop]);
      delay(1000);
    }

    // Some example procedures showing how to display to the pixels:
    colorWipe(setColor(255, 0, 0), 50); // Red
    colorWipe(setColor(0, 255, 0), 50); // Green
    colorWipe(setColor(0, 0, 255), 50); // Blue
                                        //colorWipe(setColor(0, 0, 0, 255), 50); // White RGBW
    // Send a theater pixel chase in...
    theaterChase(setColor(127, 127, 127), 50); // White
    theaterChase(setColor(127, 0, 0), 50);     // Red
    theaterChase(setColor(0, 0, 127), 50);     // Blue

    rainbow(20);
    rainbowCycle(20);
    theaterChaseRainbow(50);
  }

  //  uint16_t h = 0, s = 100, v = 25;
  //  for (int i = 0; i < NR_OF_LEDS; ++i) {
  //    set_led(stripe, i, hsv_to_rgb(h, s, v));
  //    refresh_stripe(stripe, NR_OF_LEDS);
  //    delay(5);
  //  }
}