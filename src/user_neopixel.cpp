// SPDX-FileCopyrightText: 2019 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#if 0 // Change to 1 to enable this code (must enable ONE user*.cpp only!)

#include <Adafruit_NeoPixel.h>
#define LED_PIN    2
#define LED_COUNT  8
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// #include <Adafruit_NeoPixel_ZeroDMA.h>
// #define LED_PIN   8
// #define LED_COUNT 8
// Adafruit_NeoPixel_ZeroDMA strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void user_setup(void) {
  // bool tmp = strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  for (int i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0xFF, 0x55, 0x00);
  }
  strip.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.show();

  // if (tmp = true)
  // {
  //   Serial.println("neopixel setup success");
  // }
  // else
  // {
  //   Serial.println("neopixel setup failure");
  // }
}

long firstPixelHue = 0;
long loopCnt = 0;

void user_loop(void) {
  loopCnt++;
  if (loopCnt % 10 == 0)
  {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      // strip.setPixelColor(i, 255, 0, 0);
    }
       

  // //   Serial.printf("Setting %u red\n", i);
  // // // }
  // //   strip.setBrightness(10);
  // //   strip.show(); // Update strip with new contents
  // // firstPixelHue += 256;

  } 
}

#endif // 0
