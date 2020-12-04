#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define LED_PIN    2
#define LED_COUNT 12

tmElements_t tm;

int hh = 0;
int mm = 0;
int ss = 0;

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
byte hue = 0;
byte offset = 4;
byte offset2 = 3;



void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // wait for serial

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)

  RTC.get();

  if (RTC.chipPresent())
  {
    flashColor(Wheel(0));
  }
  else
  {
    flashColor(Wheel(80));
  }
}


void loop()
{
  updateTime();
  delay(250);
}

void updateTime()
{
  RTC.read(tm);
  
  int h = (tm.Hour > 12) ? tm.Hour - 12 : tm.Hour ;
  int m = tm.Minute / 5;
  int s = tm.Second / 5;

  Serial.print("tm.Hour = ");
  Serial.print(tm.Hour);
  Serial.print(", tm.Minute = ");
  Serial.print(tm.Minute);
  Serial.print(", tm.Second = ");
  Serial.print(tm.Second);

  Serial.print(", h = ");
  Serial.print(h);
  Serial.print(", m = ");
  Serial.print(m);
  Serial.print(", s = ");
  Serial.println(s);

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
      if (i == 0)
      {
        strip.setPixelColor(i, 100, 100, 0);
      }
      else if (i == 3 || i == 6 || i == 9)
      {
        strip.setPixelColor(i, 50, 50, 50);
      }
      else
      {
        if (tm.Second % 2 != 0)
          strip.setPixelColor(i, 0, 0, 0);
        else
          strip.setPixelColor(i, 50, 50, 50);
      }
    }
  }

  strip.show();
}

void doTheLights()
{
  pushPixel(Wheel(hue));

  hue = (hue + offset) % 0xFF;

  offset = (offset + offset2) % 0x40;
  offset2 = (offset2 + 1) % 0x20;
}


void pushPixel(uint32_t color)
{
  uint16_t count = strip.numPixels();

  for (uint16_t i = 0; i < count - 1; i++)
  {
    uint32_t c = strip.getPixelColor(i + 1);
    strip.setPixelColor(i, c);
  }

  strip.setPixelColor(count - 1, color);
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void flashColor(uint32_t color)
{
  for (int x = 0; x < 5; x++)
  {
    for (int i = 0; i < 12; i++)
    {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(100);
    for (int i = 0; i < 12; i++)
    {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(100);
  }
}
