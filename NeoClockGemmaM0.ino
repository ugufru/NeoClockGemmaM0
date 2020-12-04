
// Flashing variation; only the cardinal.

#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include "RTClib.h"

#define LED_PIN    1
#define LED_COUNT 12
#define LED_BRIGHTNESS 100

#define DATAPIN   3
#define CLOCKPIN   4

RTC_PCF8523 rtc;

int hh = 0;
int mm = 0;
int ss = 0;

Adafruit_DotStar dotstar = Adafruit_DotStar(1, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

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

int loopCount = 0;


void setup()
{
  //    Serial.begin(9600);
  //    while (!Serial) ; // wait for serial
  //    Serial.println("Hello...");

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(LED_BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)

  dotstar.begin();
  dotstar.show();

  initRTC();
}

void initRTC()
{
  if (rtc.begin() == false) error(80);

  //    Serial.println("Something, something...");
  //    Serial.print("rtc.initialized() = ");
  //    Serial.println(rtc.initialized());
  //    Serial.print("rtc.lostPower() = ");
  //    Serial.println(rtc.lostPower());

  if (! rtc.initialized() || rtc.lostPower())
  {
    //   Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //
    // Note: allow 2 seconds after inserting battery or applying external power
    // without battery before calling adjust(). This gives the PCF8523's
    // crystal oscillator time to stabilize. If you call adjust() very quickly
    // after the RTC is powered, lostPower() may still return true.
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc.start();
}

void error(int pos)
{
  flashColor(Wheel(pos));
  abort();
}


void loop()
{
  loopCount = (loopCount + 1) % 50000;

  if (loopCount == 0) updateTime();
}

void updateTime()
{
  DateTime now = rtc.now();

  int h = now.twelveHour() % 12;
  int m = now.minute();
  int s = now.second();
  int hh = h;
  int mm = m / 5;
  int ss = s / 5;

  //    Serial.print(", h = ");
  //    Serial.print(h);
  //    Serial.print(", m = ");
  //    Serial.print(m);
  //    Serial.print(", s = ");
  //    Serial.print(s);
  //    Serial.print(" -|- hh = ");
  //    Serial.print(hh);
  //    Serial.print(", mm = ");
  //    Serial.print(mm);
  //    Serial.print(", ss = ");
  //    Serial.println(ss);

  // The cardinal is white and flashes.

  for (int i = 0; i < 12; i++)
  {
    if (s % 2 == 0)
    {
      if (i == 0)
      {
        strip.setPixelColor(i, 100, 100, 100);
      }
      else
      {
        if (i == ss)
        {
          int v = (s - (ss * 5)) * 32 + 64;
          strip.setPixelColor(i, 0, 0, v);
        }
        else
        {
          if (i == mm)
          {
            strip.setPixelColor(i, 0, 255, 0);
          }
          else
          {
            if (i == hh)
            {
              strip.setPixelColor(i, 255, 0, 0);
            }
            else
            {
              strip.setPixelColor(i, 0, 0, 0);
            }
          }
        }
      }
    }
    else
    {
      if (i == hh)
      {
        strip.setPixelColor(i, 255, 0, 0);
      }
      else
      {
        if (i == mm)
        {
          strip.setPixelColor(i, 0, 255, 0);
        }
        else
        {
          if (i == ss)
          {
            int v = (s - (ss * 5)) * 32 + 64;
            strip.setPixelColor(i, 0, 0, v);
            //            strip.setPixelColor(i, 0, 0, 255);
          }
          else
          {
            if ((i > 0) && (i <= hh))
            {
              strip.setPixelColor(i, 0, 0, 0);
            }
            else
            {
              strip.setPixelColor(i, 0, 0, 0);
            }
          }
        }
      }
    }
  }
  strip.show();
}

void doTheRainbowDance()
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
