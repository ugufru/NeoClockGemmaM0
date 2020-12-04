#include <Adafruit_NeoPixel.h>

#define LED_PIN    1
#define LED_COUNT 12


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


int hour = 0;
int minute = 0;
int second = 0;


void setup()
{
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}


void loop()
{
  second++;

  if (second > 59)
  {
    second = 0;
    minute++;
  }

  if (minute > 59)
  {
    minute = 0;
    hour ++;
  }

  if (hour > 11)
  {
    hour = 0;
  }

  updateTime();
  delay(1000);
}

void updateTime()
{
  int h = hour;
  int m = minute / 5;
  int s = second / 5;

  for (int i = 0; i < 12; i++)
  {
    if (i == h)
    {
      strip.setPixelColor(i, 255, 0, 0);
    }
    else if (i == m)
    {
      strip.setPixelColor(i, 0, 255, 0);
    }
    else if (i == s)
    {
      strip.setPixelColor(i, 0, 0, 255);
    }
    else
    {
      if (i == 0 || i == 3 || i == 6 || i == 9)
      {
        strip.setPixelColor(i, 50, 50, 50);
      }
      else
      {
        if (second % 2 == 0)
          strip.setPixelColor(i, 0, 0, 0);
        else
          strip.setPixelColor(i, 10, 10, 10);
      }
    }
  }

  strip.show();
}
